#include "context_tree_node.c"


typedef struct ContextTree {
  uint32_t depth;
  ContextTreeNode *root;
} ContextTree;

typedef struct CTNodeList {
  uint32_t size;
  uint32_t capacity;
  ContextTreeNode *list;
}

ContextTree *ctw_create_tree(uint32_t depth) {
  ContextTree *tree = (ContextTree *) malloc(sizeof(ContextTree));
  tree->depth = depth;
  tree->root = ctw_create_node();
  tree->l
}

class CTWContextTree:
    def __init__(self, depth):
        """ Create a context tree of specified maximum depth.
            Nodes are created as needed.
            - `depth`: the maximum depth of the context tree.
        """

        # An list used to hold the nodes in the context tree that correspond to the current context.
        # It is important to ensure that `update_context()` is called before accessing the contents
        # of this list as they may otherwise be inaccurate.
        self.context = []

        # The maximum depth of the context tree.
        # (Called `m_depth` in the C++ version.)
        assert depth >= 0, "The given tree depth must be greater than zero."
        self.depth = depth

        # The history (a list) of symbols seen by the tree.
        # (Called `m_history` in the C++ version.)
        self.history = []

        # The root node of the context tree.
        # (Called `m_root` in the C++ version.)
        self.root = CTWContextTreeNode(tree = self)

        # The size of this tree.
        self.tree_size = 1
    # end def

    def clear(self):
        """ Clears the entire context tree including all nodes and history.
        """

        # Reset the history.
        self.history = []

        # Set a new root object, and reset the tree size.
        self.root.tree = None
        del self.root
        self.root = CTWContextTreeNode(tree = self)
        self.tree_size = 1

        # Reset the context.
        self.context = []
    # end def

    def generate_random_symbols(self, symbol_count):
        """ Returns a symbol string of a specified length by sampling from the context tree.
            - `symbol_count`: the number of symbols to generate.
            (Called `genRandomSymbols` in the C++ version.)
        """
        symbol_list = self.generate_random_symbols_and_update(symbol_list, symbol_count)
        self.revert(symbol_count)

        return symbol_list
    # end def

    def generate_random_symbols_and_update(self, symbol_count):
        """ Returns a specified number of random symbols distributed according to
            the context tree statistics and update the context tree with the newly
            generated symbols.
            - `symbol_count`: the number of symbols to generate.
            (Called `genRandomSymbolsAndUpdate` in the C++ version.)
        """

        symbol_list = []
        for i in xrange(0, symbol_count):
            # Pick either 0 or 1 based on the probability of the symbol 1 occuring in the context tree.
            symbol = 1 if (random.random() < self.predict(1)) else 0
            symbol_list += [symbol]
            self.update(symbol)
        # end for

        return symbol_list
    # end def

    def predict(self, symbol_list):
        """ Returns the conditional probability of a symbol (or a list of symbols), considering the history.
            Given a history sequence `h` and a symbol `y`, the estimated probability is given by
              rho(y | h) = rho(hy)/rho(h)
            where `rho(h) = P_w^epsilon(h)` is the weighted probability estimate of observing `h`
            evaluated at the root node `epsilon` of the context tree.
            - `symbol_list` The symbol (or list of symbols) to estimate the conditional probability of.
                            0 corresponds to `rho(0 | h)` and 1 to `rho(1 | h)`.
        """

        # Ensure that we have a list, by making this a list if it's a single symbol.
        if type(symbol_list) != list:
            symbol_list = [symbol_list]
        # end if


        # If there is insufficient context for a prediction, return the uniform
        # prediction 0.5 ^ length.
        symbol_list_length = len(symbol_list)
        if ((len(self.history) + symbol_list_length) <= self.depth):
            return 0.5 if symbol_list_length == 1 else math.pow(0.5, symbol_list_length)
        # end if

        # Calculate the probability of the symbol s given the history h using
        # p(s | h) = p(hs) / p(h) = exp(ln p(hs) - ln p(h)).
        prob_history = self.root.log_probability
        self.update(symbol_list)
        prob_sequence = self.root.log_probability
        self.revert(symbol_list_length)

        return math.exp(prob_sequence - prob_history)
    # end def

    def revert(self, symbol_count = 1):
        """ Restores the context tree to its state prior to a specified number of updates.
     
            - `num_symbols`: the number of updates (symbols) to revert. (Default of 1.)
        """

        # Traverse the tree from leaf to root according to the context.
        for i in xrange(0, symbol_count):
            # Check if we have updates to revert.
            if len(self.history) == 0:
                return
            # end if

            # Get the most recent symbol and delete from the history.
            symbol = self.history.pop()

            # Traverse the tree from leaf to root according to the context. Update the
            # probabilities and symbol counts for each node. Delete unnecessary nodes.
            if len(self.history) >= self.depth:
                self.update_context()

                # Step backwards through the nodes in the context in reverse context order.
                # (Only go as deep as the current tree depth, though.)
                for context_node in reversed(self.context[:self.depth]):
                    context_node.revert(symbol)
                # end for
            # end if
        # end for
    # end def

    def revert_history(self, symbol_count = 1):
        """ Shrinks the history without affecting the context tree.
            (Called `revertHistory` in the C++ version.)
        """

        assert symbol_count > 0, "The given symbol count should be greater than 0."
        history_length = len(self.history)
        assert history_length >= symbol_count, "The given symbol count must be greater than the history length."

        new_size = history_length - symbol_count
        self.history = self.history[:new_size]
    # end def

    def size(self):
        """ Returns the number of nodes in the context tree.
        """

        # Return the value stored and updated by the children nodes.
        return self.tree_size
    # end def

    def update(self, symbol_list):
        """ Updates the context tree with a new (binary) symbol, or a list of symbols.
            Recalculates the log weighted probabilities and log KT estimates for each affected node.
            - `symbol_list`: the symbol (or list of symbols) with which to update the tree.
                              (The context tree is updated with symbols in the order they appear in the list.)
        """

        # Ensure that we have a list, by making this a list if it's a single symbol.
        if type(symbol_list) != list:
            symbol_list = [symbol_list]
        # end if

        # Traverse the tree from leaf to root according to the context.
        for symbol in symbol_list:
            # Update the probabilities and symbol counts for each node.
            symbol = int(symbol)
            if len(self.history) >= self.depth:
                self.update_context()

                # Step backwards through the nodes in the context in reverse context order.
                # (Only go as deep as the current tree depth, though.)
                for context_node in reversed(self.context[:self.depth]):
                    context_node.update(symbol)
                # end for
            # end if

            # Add this symbol to the history.
            self.update_history(symbol)
        # end for
    # end def

    def update_context(self):
        """ Calculates which nodes in the context tree correspond to the current
            context, and adds them to `context` in order from root to leaf.
            In particular, `context[0]` will always correspond to the root node
            and `context[self.depth]` corresponds to the relevant leaf node.
            Creates the nodes if they do not exist.
            (Called `updateContext` in the C++ version.)
        """

        # Ensure that the length of the history is greater than or equal to the tree depth for safety.
        assert len(self.history) >= self.depth, "The history length must be greater than the tree depth."

        # Traverse the tree from root to leaf according to the context.
        # Save the path taken and create new nodes as necessary.
        self.context = [self.root]
        node = self.root
        update_depth = 1
        for symbol in reversed(self.history):
            # Find the relevant child node of the current node for the current symbol, if it exists.
            symbol = int(symbol)
            if symbol in node.children:
                node = node.children[symbol]
            else:
                # No child exists for this symbol.

                # Create a new node for the context, and add it into the tree under the current symbol.
                new_node = CTWContextTreeNode(tree = self)
                node.children[symbol] = new_node

                # Increase the size of the tree by 1, for the new node.
                self.tree_size += 1

                # Move onto this new node.
                node = new_node
            # end if

            # Add the node to the context path.
            self.context += [node]

            # Have we hit the end of the update depth yet?
            update_depth += 1
            if update_depth > self.depth:
                # Yes. Stop updating the context.
                break
            # end if
        # end for
    # end def

    def update_history(self, symbol_list):
        """ Appends a symbol (or a list of symbols) to the tree's history without updating the tree.
            - `symbol_list`: the symbol (or list of symbols) to add to the history.
            (Called `updateHistory` in the C++ version.)
        """

        # Ensure that we have a list, by making this a list if it's a single symbol.
        if type(symbol_list) != list:
            symbol_list = [symbol_list]
        # end if

        self.history += symbol_list
    # end def
# end class
*/
