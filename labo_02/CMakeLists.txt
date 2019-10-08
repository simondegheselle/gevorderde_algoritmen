cmake_minimum_required(VERSION 3.14)
project(labo_02)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(labo_02
        .idea/workspace.xml
        searchtree.h
        SplayTree.cpp
        splaytree.h)
