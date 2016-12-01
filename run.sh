#!/bin/bash

g++ -c reversi.cpp
g++ -c main.cpp
g++ -o main reversi.o main.o
./main
