#!/bin/bash

mpic++ -std=c++11 -o main reversi.cpp treeNode.cpp main.cpp
mpirun -np 4 ./main
