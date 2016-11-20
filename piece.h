#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <utility>

enum direction{up, down, both};

class Piece {
  private:
    int x, y;
    bool black;
    bool king;
  public:
    Piece(int a, int b, bool black, bool king);
    void setBlack();
    void setWhite();
    void setKing();
    void unsetKing();
    void moveTo(int a, int b);
    bool isBlack();
    bool isKing();
    direction moveDir();
};

#endif
