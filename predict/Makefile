
repl: 
	gcc main.c ../bit_vector.c context_tree.c context_tree_node.c ctw_list.c -o repl -lm -DREPL

test: 
	gcc ctw_test.c ../bit_vector.c context_tree.c context_tree_node.c ctw_list.c -o test -lm

clean:
	rm ./repl
	rm ./test
