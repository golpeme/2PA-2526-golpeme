#pragma once
#ifndef MGMPLAYER_H
#define MGMPLAYER_H

#include "MGMpiece.h"
#include "IParcheesi.h"
#include "game.h"

class MGMPlayer : public IPlayer{
  public:
    static const int kPieceNum = 4;
    static const int kSafePoints = 12;

    MGMPlayer(int player_index);
    ~MGMPlayer() = default;

    int DecideMove(const IParcheesi& parcheesi, int player_index, int dice_roll) const override;
    
    
  private:
    IParcheesi::Color color_{IParcheesi::Color::None};
    int entry_point{ 0 };
    int safe_points[kSafePoints]{};
    int player_index_{ 0 };
    Piece piece_[kPieceNum];
};

#endif // MGMPLAYER_H














































