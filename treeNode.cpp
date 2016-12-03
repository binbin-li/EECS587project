#include "treeNode.h"
#include <time.h>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <stddef.h>
#include <math.h>

TreeNode::TreeNode(Reversi board, int height, int player, TreeNode *parentNode) {
  this->board = board;
  this->height = height;
  this->childrenNum = 0;
  this->parent = parentNode;
  this->rewards = 0;
  this->visitNum = 0;
  children = std::vector<TreeNode*>(28);
  nextMoves = std::vector<std::pair<int, int> >(28);
  std::vector<std::pair<int, int> > tmp = getValidMoves(player);
  for (int i = 0; i < tmp.size(); ++i) {
    nextMoves[i] = tmp[i];
  }
  this->moveNum = tmp.size();
}

std::vector<std::pair<int, int> > TreeNode::getValidMoves(int player) {
  return this->board.getValidMoves();
}

// Update visit number and reward from bottom to up
void TreeNode::update(int reward) {
  this->rewards += reward;
  this->visitNum += 1;
  //reward = -reward;
  TreeNode *cur = this->parent;
  while (cur) {
    cur->rewards += reward;
    cur->visitNum += 1;
    //reward = -reward;
    cur = cur->parent;
  }
}

TreeNode* TreeNode::treePolicy(int color) {
  std::vector<std::pair<int, int> > moves;
  TreeNode *node = this;
  while (true) {
    moves = node->board.getValidMoves();
    if (moves.empty()) break;
    if (node->moveNum == node->childrenNum) {
      node = node->bestChild(color);
    } else {
      return node->expand();
    }
  }
  return node;
}

TreeNode* TreeNode::expand() {
  int pos = rand() % (moveNum - childrenNum), last = moveNum - childrenNum - 1;
  std::swap(this->nextMoves[pos], this->nextMoves[last]);
  std::pair<int, int> move = this->nextMoves[last];
  Reversi nextBoard = this->board;
  nextBoard.makeMove(move.first, move.second);
  nextBoard.turnOver();
  TreeNode *newNode =
    new TreeNode(nextBoard, this->height + 1, nextBoard.getPlayer(), this);
  this->children[last] = newNode;
  ++this->childrenNum;
  return newNode;
}

TreeNode* TreeNode::bestChild(int color) {
  std::vector<TreeNode*> children = this->children;
  double maxReward = -1e8;
  TreeNode* bestNode;
  int sign = (color == this->board.getPlayer()) ? 1 : -1;
  for (int i = 0; i < this->childrenNum; ++i) {
    double curReward =
      ucb(sign * children[i]->rewards, children[i]->visitNum, this->visitNum);
    if (curReward > maxReward) {
      maxReward = curReward;
      bestNode = children[i];
    }
  }
  return bestNode;
}

std::pair<int, int> TreeNode::bestMove(int color) {
  std::pair<int, int> move;
  TreeNode *child = this->bestChild(color);
  for (int i = 0; i < this->children.size(); ++i) {
    if (child == this->children[i]) {
      return this->nextMoves[i];
    }
  }
  return move;
}

double TreeNode::ucb(int a, int b, int c) {
  double C = 1;
  return ((double) a) / ((double) b) + C * sqrt(2 * log((double) c) / (double) b);
}

int TreeNode::defaultPolicy() {
  Reversi game = this->board;
  while (true) {
    std::vector<std::pair<int, int> > moves = game.getValidMoves();
    if (moves.empty()) break;
    std::pair<int, int> move = moves[rand() % moves.size()];
    game.makeMove(move.first, move.second);
    game.turnOver();
  }
  // game.printBoard();
  int score = game.getScore(this->board.getPlayer());
  if (score > 0) return 1;
  else if (score < 0) return 0;
  else return 0;
}

void TreeNode::printParameter() {
  std::cout << "height: " << this->height <<
    ", childrenNum: " << this->childrenNum <<
    ", rewards: " << this->rewards <<
    ", visitNum: " << this->visitNum <<
    ", moveNum: " << this->moveNum << "\n";
}
