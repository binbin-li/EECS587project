#ifndef TREENODE_H_
#define TREENODE_H_

#include <vector>
#include "reversi.h"

class TreeNode {
  private:
    int height, childrenNum, rewards, visitNum, moveNum;
    Reversi board;
    TreeNode *parent;
    std::vector<TreeNode*> children;
    std::vector<std::pair<int, int> > nextMoves;
  public:
    TreeNode() {};
    TreeNode(Reversi board, int height, int player, TreeNode *parentNode);
    std::vector<std::pair<int, int> > getValidMoves(int player);
    void update(int reward);
    TreeNode* treePolicy(int color);
    TreeNode* expand();
    TreeNode* bestChild(int color);
    std::pair<int, int> bestMove(int color);
    double ucb(int a, int b, int c);
    int defaultPolicy();
    void turnOver() {this->board.turnOver();};
    void makeMove(int x, int y) {this->board.makeMove(x, y);};
    void printBoard() {this->board.printBoard();};
    void printParameter();
};

#endif
