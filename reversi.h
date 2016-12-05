#ifndef REVERSI_H_
#define REVERSI_H_

#include <vector>

class Reversi {
  private:
    int board[8][8];
    int player;
    std::vector<std::pair<int, int> > dirs;
  public:
    Reversi();
    void resetBoard();
    bool isOnBoard(int x, int y);
    std::vector<std::pair<int, int> > piecesToFlip(int xstart, int ystart);
    bool isValidMove(int xstart, int ystart);
    std::vector<std::pair<int, int> > getValidMoves();
    bool makeMove(int xstart, int ystart);
    int getScore(int color);
    int getPlayer() {return player;};
    void turnOver() {player = -player;};
    void setPlayer(int color) {player = color;};
    void setBoard();
    void printBoard();
};

#endif
