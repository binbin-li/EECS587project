#!/bin/bash

g++ -c reversi.cpp
g++ -c main.cpp
g++ -c treeNode.cpp
g++ -o main reversi.o treeNode.o main.o
./main
