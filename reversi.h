#ifndef REVERSI_H_
#define REVERSI_H_

#include <vector>

class Reversi {
  private:
    int board[8][8];
    std::vector<std::pair<int, int> > dirs;
  public:
    Reversi();
    void resetBoard();
    bool isOnBoard(int x, int y);
    std::vector<std::pair<int, int> > piecesToFlip(int color, int xstart, int ystart);
    bool isValidMove(int color, int xstart, int ystart);
    std::vector<std::pair<int, int> > getValidMoves(int color);
    bool makeMove(int color, int xstart, int ystart);
    int getScoreOfBlack();
};

#endif
