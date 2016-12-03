#include "reversi.h"
#include <vector>
#include <iostream>

Reversi::Reversi()
{
  resetBoard();
  player = -1;
  for (int i = -1; i <= 1; ++i) {
    for (int j = -1; j <= 1; ++j) {
      if (i == 0 && j == 0) continue;
      dirs.push_back(std::make_pair(i, j));
    }
  }
}

// In my setting, 1 stands for white, and -1 stands for black, 0 stands for empty
void Reversi::resetBoard()
{
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      board[i][j] = 0;
    }
  }
  board[3][3] = 1;
  board[4][4] = 1;
  board[3][4] = -1;
  board[4][3] = -1;
}

bool Reversi::isOnBoard(int x, int y)
{
  return x >= 0 && x <= 7 && y >= 0 && y <= 7;
}

std::vector<std::pair<int, int> > Reversi::piecesToFlip(int xstart, int ystart)
{
  std::vector<std::pair<int, int> > tilesToFlip;
  if (board[xstart][ystart] != 0 || !isOnBoard(xstart, ystart)) {
    return tilesToFlip;
  }
  board[xstart][ystart] = player;
  int oppositeColor = -player, x, y, xdir, ydir;

  // Find all tiles to be flipped
  for (int i = 0; i < dirs.size(); ++i) {
    std::pair<int, int> dir = dirs[i];
    x = xstart;
    y = ystart;
    xdir = dir.first;
    ydir = dir.second;
    x += xdir;
    y += ydir;
    if (isOnBoard(x, y) && board[x][y] == oppositeColor) {
      x += xdir;
      y += ydir;
      if (!isOnBoard(x, y)) {
        continue;
      }
      while (board[x][y] == oppositeColor) {
        x += xdir;
        y += ydir;
        if (!isOnBoard(x, y)) {
          break;
        }
      }
      if (!isOnBoard(x, y)) {
        continue;
      }
      if (board[x][y] == player) {
        while(1) {
          x -= xdir;
          y -= ydir;
          if (x == xstart && y == ystart) {
            break;
          }
          tilesToFlip.push_back(std::make_pair(x, y));
        }
      }
    }
  }

  board[xstart][ystart] = 0;
  return tilesToFlip;
}

bool Reversi::isValidMove(int xstart, int ystart)
{
  std::vector<std::pair<int, int> > tilesToFlip;
  tilesToFlip = piecesToFlip(xstart, ystart);
  return !tilesToFlip.empty();
}

std::vector<std::pair<int, int> > Reversi::getValidMoves()
{
  std::vector<std::pair<int, int> > validMoves;
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      if (isValidMove(i, j)) {
        validMoves.push_back(std::make_pair(i, j));
      }
    }
  }
  return validMoves;
}

bool Reversi::makeMove(int xstart, int ystart)
{
  std::vector<std::pair<int, int> > tilesToFlip;
  tilesToFlip = piecesToFlip(xstart, ystart);
  if (tilesToFlip.empty()) {
    return false;
  }
  board[xstart][ystart] = player;
  for (int i = 0; i < tilesToFlip.size(); ++i) {
    std::pair<int, int> pos = tilesToFlip[i];
    board[pos.first][pos.second] = player;
  }
  return true;
}

int Reversi::getScore(int color) {
  int score = 0;
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      if (board[i][j] == color) {
        score++;
      }
      else if (board[i][j] == -color) {
        score--;
      }
    }
  }
  return score;
}

void Reversi::printBoard() {
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      std::cout << board[i][j] << '\t';
    }
    std::cout << "\n";
  }
  std::cout << "\n";
}
