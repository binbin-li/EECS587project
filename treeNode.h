#ifndef TREENODE_H_
#define TREENODE_H_

#include <vector>
#include <unordered_map>
#include "reversi.h"

class TreeNode {
  private:
    int height, childrenNum, player, rewards, visitNum;
    Reversi board;
    TreeNode *parent;
    std::vector<TreeNode*> children(28);
    std::vector<double> rewards(28);
    std::vector<std::pair<int, int> > nextMoves(28);
  public:
    TreeNode() {};
    TreeNode(Reversi board, int height, int player);
    std::vector<std::pair<int, int> > getValidMoves(int player);
    int MC(Reversi board, int player);
    void update();
}

#endif
