#include "piece.h"

Piece::Piece(int a, int b, bool black, bool king)
{
  x = a;
  y = b;
  this->black = black;
  this->king = king;
}

void Piece::setBlack()
{
  this->black = true;
}

void Piece::setWhite()
{
  this->black = false;
}

void Piece::setKing()
{
  this->king = true;
}

void Piece::unsetKing()
{
  this->king = false;
}

void Piece::moveTo(int a, int b)
{
  x = a;
  y = b;
}

bool Piece::isBlack()
{
  return black;
}

bool Piece::isKing()
{
  return king;
}

// Black peices can only move down and white ones can move up
direction Piece::moveDir()
{
  if (this->isKing()) return both;
  if (this->isBlack()) return down;
  else return up;
}
