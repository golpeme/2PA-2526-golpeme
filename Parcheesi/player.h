#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "piece.h"

enum Colors{
  kColor_Yellow = 0,
  kColor_Blue,
  kColor_Red,
  kColor_Green
};

struct Player{
  static const int kPieceNum = 4;

  Colors color_;
  
  Piece piece_[kPieceNum];

  Player(Colors color);
  bool PiecesAtHome();
  int CountPiecesOnBox(int box);
};

#endif // __PLAYER_H__














































