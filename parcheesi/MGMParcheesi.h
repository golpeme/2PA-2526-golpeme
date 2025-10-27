#pragma once
#include "iparcheesi.h"
#include "MGMPiece.h"
#include "MGMPlayer.h"

class MGMParcheesi : public IParcheesi{
  public: 
  static const int kNumPlayers = 4;

  int EntryBox(int player_idx) const override;
  int ExitBox(int player_idx) const override;
  int PiecesAtHome(int player_idx) const override;
  int PiecesAtEnd(int player_idx) const override;
  Color ColorofPiece(int box_index, int piece_index) const override;
  bool IsBoxSafe(int box_index) const override;
  Movement ApplyMovement(int piece_index, int player_index, int count) override;
  
  
  private:
  Player players_[kNumPlayers];
  int entry_box_[kNumPlayers];
  int exit_box_[kNumPlayers];
  bool safe_[IParcheesi::board_size + 1];
  static Color ToIfaceColor_(Colors co_await);
  
};

static Color ToPieceColor_(Colors c) {
  switch (c) {
    case kColor_Yellow: return kPC_Yellow;
    case kColor_Blue:   return kPC_Blue;
    case kColor_Red:    return kPC_Red;
    case kColor_Green:  return kPC_Green;
    default:            return kPC_None;
  }
}

Color MGMParcheesi::ColorOfPiece(int box_index, int piece_index) const{
  if (box_index <= 0 || box_index > IParcheesi::board_size) return kPC_None;
  if(piece_index < 0 || piece_index > 3) return kPC_None;
  
  int found = 0;
  for (int i = 0; i < Parcheesi::kMaxPlayers; i++)
  {
    for (int j = 0; j < Player::kPieceNum; j++)
    {
      if (player_[i].piece_[j].position == box_index)
      {
        if(found == piece_index){
          return ToPieceColor_(player_[i].color_);
        }
        found++;
        if(found > 1) break;
      }
    }
    if(found > piece_index) break;
  }
  return kPC_None;
}

//Read Only Helpers
static int PieceOnBox(const IParcheesi& board, int box, Color* color0, Color* color1){
  if (color0) *color0 = kPC_None;
  if (color1) *color1 = kPC_None;
  
  
  Color a = board.ColorOfPiece(box, 0);
  Color b = board.ColorOfPiece(box, 1);

  int count = 0;
  if(a != kPC_None) {
    if(color0){
      *color0 = a;
      count++;
    }
  }
  if(b != kPC_None){
    if(count == 0 && color0){
      *color0 = b;
    }else if(color1){
      *color1 = b;
      count++;
    }
  }
  return count;
}

static int IsBridge(const IParcheesi& board, int box, Color same){
  
}

static int EntryBlocked(const IParcheesi& board, int player, Color same){

}

static int NextOnRing(int from, int steps){

}

static bool CanMove(const IParcheesi& board, Color own, int dest, bool* will_eat){

}