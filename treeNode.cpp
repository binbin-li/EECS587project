#include "treeNode.h"
#include <time.h>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

TreeNode::TreeNode(Reversi board, int height, TreeNode *parentNode) {
  this->board = board;
  this->height = height;
  this->childrenNum = 0;
  this->parent = parentNode;
  this->rewards = 0;
  this->visitNum = 0;
  children = std::vector<TreeNode*>(28);
  childVisitNum = std::vector<int>(28, 0);
  nextMoves = std::vector<std::pair<int, int> >(28);
  std::vector<std::pair<int, int> > tmp = this->board.getValidMoves();
  for (int i = 0; i < tmp.size(); ++i) {
    nextMoves[i] = tmp[i];
  }
  this->moveNum = tmp.size();
}

// Update visit number and reward from bottom to up
void TreeNode::update(double reward) {
  this->rewards += reward;
  this->visitNum += 1;
  TreeNode *cur = this->parent;
  while (cur) {
    reward = 1 - reward;
    cur->rewards += reward;
    cur->visitNum += 1;
    cur = cur->parent;
  }
}

TreeNode* TreeNode::treePolicy() {
  std::vector<std::pair<int, int> > moves;
  TreeNode *node = this;
  while (true) {
    moves = node->board.getValidMoves();
    if (moves.empty()) break;
    if (node->moveNum == node->childrenNum) {
      node = node->bestChild();
    } else {
      return node->expand();
    }
  }
  return node;
}

TreeNode* TreeNode::expand() {
  std::vector<int> unvisitedChild;
  for (int i = 0; i < moveNum; ++i) {
    if (childVisitNum[i] == 0) unvisitedChild.push_back(i);
  }
  int expandingIndex = unvisitedChild[rand() % unvisitedChild.size()];
  childVisitNum[expandingIndex] = 1;
  std::pair<int, int> move = this->nextMoves[expandingIndex];
  Reversi nextBoard = this->board;
  nextBoard.makeMove(move.first, move.second);
  nextBoard.turnOver();
  TreeNode *newNode =
    new TreeNode(nextBoard, this->height + 1, this);
  children[expandingIndex] = newNode;
  ++childrenNum;
  return newNode;
}

TreeNode* TreeNode::bestChild() {
  std::vector<TreeNode*> children = this->children;
  double maxReward = -1e8;
  int index = 0;
  for (int i = 0; i < this->moveNum; ++i) {
    double curReward =
      ucb(children[i]->rewards, children[i]->visitNum, this->visitNum);
    if (curReward > maxReward) {
      maxReward = curReward;
      index = i;
    }
  }
  return children[index];
}

std::pair<int, int> TreeNode::bestMove() {
  std::pair<int, int> move;
  TreeNode *child = this->bestChild();
  for (int i = 0; i < this->children.size(); ++i) {
    if (child == this->children[i]) {
      return this->nextMoves[i];
    }
  }
  return move;
}

double TreeNode::ucb(double a, double b, double c) {
  double C = 0.2;
  return 1 - a / b + C * sqrt(2 * log(c) / b);
}

double TreeNode::defaultPolicy() {
  Reversi game = this->board;
  while (true) {
    std::vector<std::pair<int, int> > moves = game.getValidMoves();
    if (moves.empty()) break;
    std::pair<int, int> move = moves[rand() % moves.size()];
    game.makeMove(move.first, move.second);
    game.turnOver();
  }
  int score = game.getScore(this->board.getPlayer());
  if (score > 0) return 1;
  else if (score < 0) return 0;
  else return 0.5;
}

void TreeNode::printParameter() {
  std::cout << "height: " << this->height <<
    ", childrenNum: " << this->childrenNum <<
    ", rewards: " << this->rewards <<
    ", visitNum: " << this->visitNum <<
    ", moveNum: " << this->moveNum << "\n";
}

TreeNode::~TreeNode() {
}

void TreeNode::deleteTree() {
  if (this != NULL) {
    for (int i = 0; i < this->moveNum; ++i) {
      this->children[i]->deleteTree();
    }
    delete this;
  }
}

std::vector<double> TreeNode::gatherData(int height) {
  vector<double> result;
  result.push_back(visitNum);
  for (int i = 0; i < childrenNum; ++i) {
    result.push_back(children[i]->visitNum);
  }
  return result;
}
