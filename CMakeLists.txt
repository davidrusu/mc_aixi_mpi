cmake_minimum_required(VERSION 3.5)
project(mc_aixi_mpi)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
#add_definitions(-D ENV_COIN_FLIP)
add_definitions(-D ENV_1D_MAZE)

set(SOURCE_FILES
        _utils/macros.h
        _utils/types.h
        agent/agent.c
        agent/agent.h
        #environment/1d_maze.c
        #environment/1d_maze.h
        environment/coin_flip.c
        environment/coin_flip.h
        predict/context_tree.c
        predict/context_tree.h
        predict/context_tree_node.c
        predict/context_tree_node.h
        predict/ctw_list.c
        predict/ctw_list.h
        predict/ctw_test.c
        predict/main.c
        search/dict.c
        search/dict.h
        search/monte_node.h
        search/search.c
        bit_vector.c
        bit_vector.h
        LICENSE
        main.c
        README.md
        utils.c environment/env.h)

add_executable(mc_aixi_mpi ${SOURCE_FILES})