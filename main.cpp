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
pair<int, int> UCT(Reversi board, double seconds, int rank, int size);
int MC(Reversi board, int player, int rank, int size);
double ucb(double a, double b, double c);
int bestIndex(TreeNode *root);

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

pair<int, int> UCT(Reversi board, double seconds, int rank, int size) {
  pair<int, int> result;
  clock_t startTime = clock();
  TreeNode *root = new TreeNode(board, 0, NULL);
  // UCT step
  while (clock() - startTime < seconds * CLOCKS_PER_SEC) {
    TreeNode *nextState = root->treePolicy();
    double reward = nextState->defaultPolicy();
    nextState->update(reward);
  }
  int childrenNum = root->childrenNum;
  if (rank == 0) {
    // Receive data from other proc
    vector<vector<double> > data(size - 1, vector<double>(childrenNum * 2 + 1));
    for (int receiver = 1; receiver < size; ++receiver) {
      MPI_Status status;
      MPI_Recv(&data[receiver - 1].front(), childrenNum * 2 + 1,
          MPI_DOUBLE, receiver, 1, MPI_COMM_WORLD, &status);
    }
    // Average data
    double sum = root->visitNum;
    for (int i = 0; i < size - 1; ++i) {
      sum += data[i][0];
    }
    for (int i = 1; i < childrenNum * 2 + 1; ++i) {
      double rewardSum = 0, visitSum = 0;
      if (i % 2 == 1) {
        // Sum up visitNum
        for (int j = 0; j < size - 1; ++j) {
          visitSum += data[j][i];
        }
        visitSum += root->children[(i - 1) / 2]->visitNum;
        root->children[(i - 1) / 2]->visitNum = visitSum / size;
      } else {
        // Sum up reward
        for (int j = 0; j < size - 1; ++j) {
          rewardSum += data[j][i];
        }
        rewardSum += root->children[i / 2 - 1]->rewards;
        root->children[i / 2 - 1]->rewards = rewardSum / size;
      }
    }
  } else {
    // Send data to proc 0
    vector<double> data = root->gatherData(0);
    MPI_Request request;
    MPI_Isend(&data.front(), childrenNum * 2 + 1, MPI_DOUBLE,
        0, 1, MPI_COMM_WORLD, &request);
  }
  if (rank == 0) {
    // int index = bestIndex(root);
    // result = root->nextMoves[index];
    result = root->bestMove();
  }
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
      move = UCT(board, 4, rank, size);
    } else {
      move = nextMoves[rand() % nextMoves.size()];
    }
    // Send the next move to other proc
    if (rank == 0) {
      cout << move.first << ' ' << move.second << endl;
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

double ucb(double a, double b, double c) {
  double C = 0.2;
  return 1 - a / b + C * sqrt(2 * log(c) / b);
}

int bestIndex(TreeNode *root) {
  cout << "start\n";
  std::vector<TreeNode*> children = root->children;
  double maxReward = -1e8;
  int index = 0;
  for (int i = 0; i < root->childrenNum; ++i) {
    double curReward =
      ucb(children[i]->rewards, children[i]->visitNum, root->visitNum);
    if (curReward > maxReward) {
      maxReward = curReward;
      index = i;
    }
  }
  cout << "end\n";
  return index;
}
