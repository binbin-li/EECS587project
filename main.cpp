#include <iostream>
#include <mpi.h>
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

pair<int, int> UCT(Reversi board) {
  pair<int, int> result;
  clock_t startTime;
  startTime = clock();
  TreeNode *root = new TreeNode(board, 0, NULL);
  /*
  for (int i = 0; i < 4000; ++i) {
    TreeNode *nextState = root->treePolicy();
    double reward = nextState->defaultPolicy();
    nextState->update(reward);
  }
  */
  while (clock() - startTime < 4 * CLOCKS_PER_SEC) {
    TreeNode *nextState = root->treePolicy();
    for (int i = 0; i < 100; ++i) {
      double reward = nextState->defaultPolicy();
      nextState->update(reward);
    }
  }
  result = root->bestMove();
  root->deleteTree();
  return result;
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
