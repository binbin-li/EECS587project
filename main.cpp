#include <iostream>
#include <math.h>
#include <time.h>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include "reversi.h"
using namespace std;

double score(double a, double b, double c);
vector<pair<int, int> > UCT(Reversi board);
double simulate(Reversi board, int player);

int main() {
  srand(time(NULL));
  Reversi board;
  simulate(board, -1);
  //UCT(board);
  return 0;
}


double score(double a, double b, double c) {
  double val = 1;
  return (a/b) + sqrt(2 * log(c) / b) * 2 * val;
}

vector<pair<int, int> > UCT(Reversi board) {
  vector<pair<int, int> > result;
  clock_t startTime;
  startTime = clock();
  while (clock() - startTime < CLOCKS_PER_SEC) {

  }
  return result;
}

double simulate(Reversi board, int player) {
  board.setPlayer(player);
  while (true) {
    vector<pair<int, int> > nextMoves = board.getValidMoves();
    if (nextMoves.empty()) break;
    int nextMoveIdx = (rand() % nextMoves.size());
    pair<int, int> nextMove = nextMoves[nextMoveIdx];
    board.makeMove(nextMove.first, nextMove.second);
    board.turnOver();
  }
  return (board.getScore(player) > 0) ? 1.0 : 0.0;
}
