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
int MC(Reversi board, int player, int rank, int size);

int main() {
  int rank, size;
  srand(time(NULL));
  MPI_Init(0, 0);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  Reversi board;
  int count = 0;
  for (int i = 0; i < 1; ++i) {
    if (MC(board, -1, rank, size) > 0) ++count;
    cout << i+1 << ' ' << count << endl;
  }

  MPI_Finalize();
  return 0;
}

pair<int, int> UCT(Reversi board, double seconds) {
  pair<int, int> result;
  clock_t startTime = clock();
  TreeNode *root = new TreeNode(board, 0, NULL);
  // Receive the data from other proc
  /*
  if (rank == 0) {
    vector<vector<double> > data(size - 1, vector<double>(65));
    double **data = new double * [size - 1];
    for (int i = 0; i < size - 1; ++i) {
      data[i] = new double [65];
    }
    for (int receiver = 1; receiver < size; ++i) {
      MPI_Status status;
      MPI_Recv(data[receiver - 1], 65, MPI_DOUBLE, 1, MPI_COMM_WORLD, &status);
    }
    for (int i = 0; i < size - 1; ++i) {
      delete data[i];
    }
    delete [] data;
  } else {
    // Send the data to the proc 0

  }
  */
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

int MC(Reversi board, int player, int rank, int size) {
  board.setPlayer(player);
  pair<int, int> move;
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
      int sendingMove[2];
      sendingMove[0] = move.first;
      sendingMove[1] = move.second;
      for (int receiver = 1; receiver < size; ++receiver) {
        MPI_Request request;
        MPI_Isend(&sendingMove, 2, MPI_INT, receiver, 0, MPI_COMM_WORLD, &request);
      }
    }
    else {
      // Receive the next move from proc 0
      int receivingMove[2];
      MPI_Status status;
      MPI_Recv(&receivingMove, 2, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
      move.first = receivingMove[0];
      move.second = receivingMove[1];
    }
    cout << move.first << ' ' << move.second << endl;
    board.makeMove(move.first, move.second);
    board.turnOver();
  }
  return board.getScore(player);
}

