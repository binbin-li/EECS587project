#include "treeNode.h"
#include <stddef.h>

TreeNode::TreeNode(Reversi board, int height, int player) {
  this->board = board;
  this->height = height;
  this->player = player;
  this->childrenNum = 0;
  this->parent = NULL;
  this->rewards = 0;
  this->visitNum = 0;
  std::vector<std::pair<int, int> > tmp = getValidMoves(player);
  std::copy(tmp.begin(), tmp.end(), this->nextMoves.begin());
}

std::vector<std::pair<int, int> > TreeNode::getValidMoves(int player) {
  return this->board.getValidMoves();
}

// Simulate MC one time and return the final reward
int TreeNode::MC(Reversi board, int player) {
  board.setPlayer(player);
  while (true) {
    std::vector<std::pair<int, int> > nextMoves = board.getValidMoves();
    if (nextMoves.empty()) break;
    int nextMoveIdx = (rand() % nextMoves.size());
    std::pair<int, int> nextMove = nextMoves[nextMoveIdx];
    board.makeMove(nextMove.first, nextMove.second);
    board.turnOver();
  }
  return (board.getScore(player) > 0) ? 1 : 0;
}

// Update visit number and reward from bottom to up
void TreeNode::update() {

}
