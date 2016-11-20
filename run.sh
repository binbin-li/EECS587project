#!/bin/bash

g++ -c piece.cpp
g++ -c main.cpp
g++ -o main piece.o main.o
./main
