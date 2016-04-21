cmake_minimum_required(VERSION 3.3)
project(mc_aixi_mpi)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    agent/agent.c
    environment/environment.c
    predict/context_tree.c
    predict/context_tree_node.

    search/search.c
    summary.c
    utils.c search/dict.c search/monte_node.c main.c)

add_executable(mc_aixi_mpi ${SOURCE_FILES})