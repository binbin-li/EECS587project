#include <iostream>
#include <math.h>
#include <time.h>
#include <iomanip>
#include <stdlib.h>
#include <stdio.h>
#include "reversi.h"
#include "treeNode.h"
using namespace std;

double score(double a, double b, double c);
pair<int, int> UCT(Reversi board);
double simulate(Reversi board, int player);
int MC(Reversi board, int player);

int main() {
  srand(time(NULL));
  Reversi board;
  //simulate(board, -1);
  // UCT(board);
  int count = 0;
  for (int i = 0; i < 100; ++i) {
    if (MC(board, -1) > 0) ++count;
    cout << i+1 << ' ' << count << endl;
  }
  cout << count << endl;
  return 0;
}


double score(double a, double b, double c) {
  double val = 1;
  return (a/b) + sqrt(2 * log(c) / b) * 2 * val;
}

pair<int, int> UCT(Reversi board) {
  vector<pair<int, int> > result;
  clock_t startTime;
  startTime = clock();
  TreeNode *root = new TreeNode(board, 0, -1, NULL);
  int color = board.getPlayer();
  for (int i = 0; i < 2000; ++i) {
    TreeNode *nextState = root->treePolicy(color);
    int reward = nextState->defaultPolicy();
    nextState->update(reward);
  }
  return root->bestMove(color);
  /*
  while (clock() - startTime < CLOCKS_PER_SEC) {

  }
  */
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

int MC(Reversi board, int player) {
  board.setPlayer(player);
  pair<int, int> move;
  while (true) {
    vector<pair<int, int> > nextMoves = board.getValidMoves();
    if (nextMoves.empty()) break;
    if (board.getPlayer() == player) {
      move = UCT(board);
    } else {
      move = nextMoves[rand() % nextMoves.size()];
    }
    board.makeMove(move.first, move.second);
    board.turnOver();
  }
  return board.getScore(player);
}
