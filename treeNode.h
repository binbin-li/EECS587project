#ifndef TREENODE_H_
#define TREENODE_H_

#include <vector>
#include "reversi.h"

class TreeNode {
  private:
    Reversi board;
    TreeNode *parent;
  public:
    int height, childrenNum, moveNum;
    double rewards, visitNum;
    std::vector<TreeNode*> children;
    std::vector<int> childVisitNum;
    std::vector<std::pair<int, int> > nextMoves;

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
    std::vector<std::pair<int, int> > getValidMoves() {
      return this->board.getValidMoves();
    };
    ~TreeNode();
    void deleteTree();
    std::vector<double> gatherData(int height);
};

#endif
