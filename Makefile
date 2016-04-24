#//////////////////////////////////////////////////////////////////
# AUTHOR:   Robert Morouney <069001422>
# EMAIL:    robert@morouney.com 
# FILE:     Makefile_2
# CREATED:  2016-04-22 18:13:19
# MODIFIED: 2016-04-23 23:34:43
#//////////////////////////////////////////////////////////////////
MCC = mpicc
CC = gcc
CFLAGS = -lm -g -std=c99 -Wall
OUT_NAME = main_aixi
default: SERIAL

MPI_DEBUG: coin_flip.o environment.o class.o agent.o context_tree.o context_tree_node.o ctw_list.o dict.o search.o bit_vector.o main_dm.o
	$(MCC) $(CFLAGS) -D DEBUG -D USE_MPI -o $(OUT_NAME).'mpi.debug' coin_flip.o environment.o class.o agent.o context_tree.o context_tree_node.o ctw_list.o dict.o search.o bit_vector.o main.o

DEBUG: coin_flip.o environment.o class.o agent.o context_tree.o context_tree_node.o ctw_list.o dict.o search.o bit_vector.o main_d.o
	$(CC) $(CFLAGS) -D DEBUG -o $(OUT_NAME).'serial.debug' coin_flip.o environment.o class.o agent.o context_tree.o context_tree_node.o ctw_list.o dict.o search.o bit_vector.o main.o

MPI: coin_flip.o environment.o class.o agent.o context_tree.o context_tree_node.o ctw_list.o dict.o search.o bit_vector.o main_m.o
	$(MCC) $(CFLAGS) -D USE_MPI -o $(OUT_NAME).'mpi' coin_flip.o environment.o class.o agent.o context_tree.o context_tree_node.o ctw_list.o dict.o search.o bit_vector.o main.o

SERIAL: coin_flip.o environment.o class.o agent.o context_tree.o context_tree_node.o ctw_list.o dict.o search.o bit_vector.o main.o
	$(CC) $(CFLAGS) -o $(OUT_NAME).'serial' coin_flip.o environment.o class.o agent.o context_tree.o context_tree_node.o ctw_list.o dict.o search.o bit_vector.o main.o

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

ctw_list.o: predict/ctw_list.c predict/ctw_list.h predict/context_tree_node.h
	$(CC) $(CFLAGS) -c predict/ctw_list.c

dict.o: _utils/types.h search/dict.c search/dict.h search/monte_node.h
	$(CC) $(CFLAGS) -c search/dict.c

search.o: search/search.c search/dict.h search/monte_node.h agent/agent.h environment/environment.h environment/environment.r environment/class.h environment/class.r _utils/types.h predict/context_tree_node.h predict/context_tree.h bit_vector.h
	$(CC) $(CFLAGS) -c search/search.c

bit_vector.o: bit_vector.c bit_vector.h
	$(CC) $(CFLAGS) -c bit_vector.c

main.o: main.c environment/environment.h environment/environment.r environment/class.h environment/class.r agent/agent.h

main_dm.o: main.c environment/environment.h environment/environment.r environment/class.h environment/class.r agent/agent.h
	$(MCC) $(CFLAGS) -D DEBUG -D MPI -c main.c

main_d.o: main.c environment/environment.h environment/environment.r environment/class.h environment/class.r agent/agent.h
	$(MCC) $(CFLAGS) -D DEBUG -c main.c

main_m.o: main.c environment/environment.h environment/environment.r environment/class.h environment/class.r agent/agent.h
	$(MCC) $(CFLAGS) -D MPI -c main.c

clean:
	$(RM) $(OUT_NAME) *.o *~

