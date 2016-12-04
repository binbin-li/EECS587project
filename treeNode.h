#ifndef TREENODE_H_
#define TREENODE_H_

#include <vector>
#include "reversi.h"

class TreeNode {
  private:
    int height, childrenNum, moveNum;
    double rewards, visitNum;
    Reversi board;
    TreeNode *parent;
    std::vector<TreeNode*> children;
    std::vector<int> childVisitNum;
    std::vector<std::pair<int, int> > nextMoves;
  public:
    TreeNode() {};
    TreeNode(Reversi board, int height, TreeNode *parentNode);
    void update(double reward);
    TreeNode* treePolicy();
    TreeNode* expand();
    TreeNode* bestChild();
    std::pair<int, int> bestMove();
    double ucb(double a, double b, double c);
    double defaultPolicy();
    void turnOver() {this->board.turnOver();};
    void makeMove(int x, int y) {this->board.makeMove(x, y);};
    void printBoard() {this->board.printBoard();};
    void printParameter();
    /*
    void updateValue(double value);
    TreeNode* descendByUCB();
    void TreeNode::playOneSequence();
    */
    std::vector<std::pair<int, int> > getValidMoves() {
      return this->board.getValidMoves();
    };
};

#endif
