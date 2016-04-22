#//////////////////////////////////////////////////////////////////
# AUTHOR:   Robert Morouney <069001422>
# EMAIL:    robert@morouney.com 
# FILE:     Makefile_2
# CREATED:  2016-04-22 18:13:19
# MODIFIED: 2016-04-22 19:04:56
#//////////////////////////////////////////////////////////////////

CC = gcc
CFLAGS = -g -Wall
OUT_NAME = main_aixi
default: main_aixi

main_aixi: coin_flip.o environment.o class.o agent.o context_tree.o context_tree_node.o ctw_iterator.o ctw_list.o dict.o search.o bit_vector.o
	$(CC) $(CFLAGS) -o $(OUT_NAME) coin_flip.o environment.o class.o agent.o context_tree.o context_tree_node.o ctw_iterator.o ctw_list.o dict.o search.o bit_vector.o

coin_flip.o: environment/coin_flip.c environment/coin_flip.h environment/coin_flip.r environment/environment.h environment/environment.r environment/class.h environment/class.r _utils/macros.h _utils/types.h
	$(CC) $(CFLAGS) -c environment/coin_flip.c

environment.o: environment/environment.c environment/environment.h environment/environment.r environment/class.h environment/class.r _utils/macros.h _utils/types.h
	$(CC) $(CFLAGS) -c environment/environment.c

class.o: environment/class.c environment/class.h environment/class.r _utils/macros.h _utils/types.h
	$(CC) $(CFLAGS) -c environment/class.c

agent.o: agent/agent.c agent/agent.h environment/environment.r environment/environment.h environment/class.r environment/class.h predict/context_tree.h _utils/types.h _utils/macros.h search/monte_node.h bit_vector.h
	$(CC) $(CFLAGS) -c agent/agent.c

context_tree.o: predict/context_tree.c predict/context_tree.h predict/context_tree_node.h predict/ctw_list.h
	$(CC) $(CFLAGS) -c predict/context_tree.c

context_tree_node.o: predict/context_tree_node.c predict/context_tree_node.h
	$(CC) $(CFLAGS) -c predict/context_tree_node.c

ctw_iterator.o: predict/ctw_iterator.c
	$(CC) $(CFLAGS) -c predict/ctw_iterator.c

ctw_list.o: predict/ctw_list.c predict/ctw_list.h predict/context_tree_node.h
	$(CC) $(CFLAGS) -c predict/ctw_list.c

dict.o: search/dict.c search/dict.h search/monte_node.h
	$(CC) $(CFLAGS) -c search/dict.c

search.o: search/search.c search/dict.h search/monte_node.h agent/agent.h environment/environment.h environment/environment.r environment/class.h environment/class.r _utils/types.h
	$(CC) $(CFLAGS) -c search/search.c

bit_vector.o: bit_vector.c bit_vector.h
	$(CC) $(CFLAGS) -c bit_vector.c

clean:
	$(RM) $(OUT_NAME) *.o *~

