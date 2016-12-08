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
pair<int, int> UCT(Reversi board, double seconds);
int MC(Reversi board, int player);
int rank, size;

int main() {
  srand(time(NULL));
  MPI_Init(0, 0);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  Reversi board;
  int count = 0;
  for (int i = 0; i < 1; ++i) {
    if (MC(board, -1) > 0) ++count;
    cout << i+1 << ' ' << count << endl;
  }

  MPI_Finalize();
  return 0;
}

pair<int, int> UCT(Reversi board, double seconds) {
  pair<int, int> result;
  clock_t startTime = clock();
  TreeNode *root = new TreeNode(board, 0, NULL);

  if (rank == 0) {
    board
    for (int receiver = 1; receiver < size; ++i) {

    }
  }
  while (clock() - startTime < seconds * CLOCKS_PER_SEC) {
    TreeNode *nextState = root->treePolicy();
    double reward = nextState->defaultPolicy();
    nextState->update(reward);
  }
  result = root->bestMove();
  root->gatherData(0);
  root->deleteTree();
  return result;
}

int MC(Reversi board, int player) {
  board.setPlayer(player);
  pair<int, int> move;
  int sendingMove[2];
  while (true) {
    vector<pair<int, int> > nextMoves = board.getValidMoves();
    if (nextMoves.empty()) break;
    if (board.getPlayer() == player) {
      move = UCT(board, 4);
    } else {
      move = nextMoves[rand() % nextMoves.size()];
    }
    // Send the next move to other proc
    if (rank == 0) {
      sendingMove[0] = move.first;
      sendingMove[1] = move.second;
      for (int receiver = 1; receiver < size; ++receiver) {
        MPI_Isend()
      }
    }
    board.makeMove(move.first, move.second);
    board.turnOver();
  }
  return board.getScore(player);
}

