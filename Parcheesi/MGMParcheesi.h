#pragma once
#include "iparcheesi.h"
#include "MGMPlayer.h"
#include "MGMPiece.h"

class MGMParcheesi : public IParcheesi{
  public: 
  static const int kNumPlayers = 4;
  static const int kBoxNum = 68;
  static const int kSafePoints = 12;
  static const int kPiecesPerPlayer = 4;
  static const int kPieceNum = kNumPlayers * kPiecesPerPlayer; //4 pieces per player

  MGMParcheesi();

  static const int MGMParcheesi::safePoint[kSafePoints];
  static const int MGMParcheesi::startPoint[kNumPlayers];
  static const int MGMParcheesi::finishPoint[kNumPlayers];

  int EntryBox(int player_idx) const override;
  int ExitBox(int player_idx) const override;
  int PiecesAtHome(int player_idx) const override;
  int PiecesAtEnd(int player_idx) const override;
  bool IsBoxSafe(int box_index) const override;
  Color ColorofPiece(int box_index, int piece_index) const override;//

  Movement ApplyMovement(int piece_index, int player_index, int count) override;
  void SendPieceHome(int piece_index, int player_index) override;
  
  MGMParcheesi* Clone() const override;

  private:
  
  int pos_[kPieceNum]{};
  static int CalcIndex(int player_index, int piece_index) { return player_index * kPiecesPerPlayer + piece_index; }
  int end_count_[kNumPlayers]{};
  
  void MovePiece(int piece_index, int player_index, int target_pos);
  int CountPiecesOnBox(int pos) const;//
  bool CheckBoxBridge(int box_index) const;

  IParcheesi::Movement ManageRunway(int piece_index, int player_index, int count);
  bool InRunway(int piece_index, int pos) const;
};