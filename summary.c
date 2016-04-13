//------------- Environment -----------------
typedef uint32_t Action;
typedef int32_t Reward;

typedef struct Environment {
  // We don't care what the name of the actions are, we just enumerate them as ints from 0 to (num_actions - 1)
  uint32_t num_actions = 3;
} Environment;


typedef struct Percept {
  bool *observation; // an observation is a bit string
  Reward reward;
} Percept;
  
Action maximum_action(Environment *env) {
  // returns the largest (ordinal) action
}

uint32_t action_bits(Environment *env) {
  // returns the number of bits required for an action
}

uint32_t percept_bits(Environment *env) {
  // returns bits required for an observation.
  // Should be observation bits + reward bits
}

Percept perform_action(Environment *env, Action action) {
  switch (action) {
  case 0: // No-op
    break;
  case 1: // Left
    // update the environment
    break;
  case ...
  }
  // create and return a perception ie. observation and reward.
  return percept;
}

//------------- Agent ----------------

typedef struct Agent {
  uint32_t age;               // how many experiences this agent has had
  uint32_t horizon;           // the monte carlo search horizon
  uint32_t mc_simulations;    // how many monte carlo simulations to perform
  ContextTree *context_tree;  // this is the agents internal model of the env
} Agent;


Agent *create_agent(Environment *environment, uint32_t ct_depth, uint32_t mc_simulations) {
  // need to construct an agent here
}

Reward playout(Agent *agent, uint32_t horizon) {
  // Randomly interact with the environment `horizon` steps into the future
  // return the total reward accumulated through playout
}

Action search(Agent *agent) {
  // use monte carlo search to find the best action
}

//------------------ CTW ----------------

typedef struct ContextTree {
  
} ContextTree;



double log_kt_multiplier(ContextTreeNode *node, bool symbol) {
  if (symbol) {
    // symbol is 1
  } else {
    // symbol is 1
  }
  ...
}

void update_ct_node(ContextTreeNode *node, bool symbol) {
  /* Updates the node after having observed a new symbol.
     This involves updating the symbol counts and recalculating the cached probabinlities.

     - `symbol`: the symbol that was observed.
  */
}


void update_ct(ContextTree *tree, bool *symbols, uint32_t num_symbols) {
  // Update the context tree with some new symbols
  
}

void update_context_ct(ContextTree *tree) {
  // Calculates which nodes in the context tree correspond to the current
  // context, and adds them to `context` in order from root to leaf.
}

void update_histor_ct(ContextTree *tree, bool *symbols, uint32_t num_symbols) {
  // adds the given symbols to the history of the context tree
  
}
double predict_ct(ContextTree *tree, bool *symbols, uint32_t num_symbols) {
  // returns the probability of the list of symbols given the past history
  // in the context tree
  ...
}

// two revert function for reverting to prior states of the context tree

// ------------- Monte Carlo Tree Search -----------

// this is the parallel portion of the code
