#include "MGMParcheesi.h"


const int MGMParcheesi::safePoint[kSafePoints] = { 5, 12, 17, 22, 29, 34, 39, 46, 51, 56, 63, 68 };
const int MGMParcheesi::startPoint[kNumPlayers] = { 5, 22, 39, 56 };
const int MGMParcheesi::finishPoint[kNumPlayers] = { 68, 17, 34, 51 };

MGMParcheesi::MGMParcheesi() {
  for (int i = 0; i < kPieceNum; i++)
  {
    pos_[i] = 0;
  }
}

int MGMParcheesi::EntryBox(int player_idx) const{
    return MGMParcheesi::startPoint[player_idx];
}

int MGMParcheesi::ExitBox(int player_idx) const {
    return MGMParcheesi::finishPoint[player_idx];
}


int MGMParcheesi::CountPiecesOnBox(int pos) const{
  int pieces_at_box = 0;
  for (int i = 0; i < kPieceNum; i++)
  {
    if (pos_[i] == pos)
    {
      pieces_at_box++;
    }
  }
  return pieces_at_box;
}

bool MGMParcheesi::CheckBoxBridge(int box_index) const{
  if (box_index <= 0 || box_index > kBoxNum) return false;

  for (int i = 0; i < kNumPlayers; i++) {
    int same_color_count = 0;
    for (int j = 0; j < kPiecesPerPlayer; j++)
    {
      const int pos = pos_[CalcIndex(i, j)];
      if (pos == box_index) {
        same_color_count++;
        if (same_color_count >= 2) return true;
      }
    }
  }
  return false;
}


int MGMParcheesi::PiecesAtHome(int player_idx) const {
    int found = 0;
    for (int i = 0; i < kPiecesPerPlayer; i++)
    {
        if (pos_[CalcIndex(player_idx, i)] == 0) found++;
    }
    return found;
}

int MGMParcheesi::PiecesAtEnd(int player_index) const {
  return end_count_[player_index];
}

bool MGMParcheesi::IsBoxSafe(int box_index) const {
    for (int i = 0; i < kSafePoints; i++)
    {
        if (box_index == safePoint[i]) return true;
    }
    return false;
}

void MGMParcheesi::SendPieceHome(int piece_index, int player_index) {
    pos_[CalcIndex(player_index, piece_index)] = 0;
}

void MGMParcheesi::MovePiece(int piece_index, int player_index, int target_pos){
    pos_[CalcIndex(player_index, piece_index)] = target_pos;
}

IParcheesi::Color MGMParcheesi::ColorofPiece(int box_index, int piece_index) const {
  int found_piece = 0;
  for (int i = 0; i < kNumPlayers; i++)
  {
    for (int j = 0; j < kPiecesPerPlayer; j++)
    {
      int index = CalcIndex(i, j);
      if (pos_[index] == box_index)
      {
        if (found_piece == piece_index) {
          switch (i)
          {
          case 0:
            return Color::Yellow;
            break;
          case 1:
            return Color::Blue;
            break;
          case 2:
            return Color::Red;
            break;
          case 3:
            return Color::Green;
            break;

          default:
            return Color::None;
            break;
          }
        }
        found_piece++;
      }
    }
  }
  return Color::None;
}

bool MGMParcheesi::InRunway(int player, int pos) const {
  int exit_box = ExitBox(player);
  return pos > exit_box && pos <= kBoxNum + exit_size;
}

IParcheesi::Movement MGMParcheesi::ManageRunway(int piece_index, int player_index, int count) {
  int index = CalcIndex(player_index, piece_index);
  int& current_pos = pos_[index];
  int exit_box = ExitBox(player_index);

  int mov = current_pos - exit_box;
  int target_mov = mov + count;

  if (target_mov < exit_size) {
    current_pos = exit_box + target_mov;
    return Movement::Normal;
  }
  else if (target_mov == exit_size) {
    current_pos = -1;
    end_count_[player_index]++;
    return Movement::End;
  }
  else {
    return Movement::IllegalPastEnd;
  }
}

IParcheesi::Movement  MGMParcheesi::ApplyMovement(int piece_index, int player_index, int count){

  /*Player& turn_player = players_[player_index];
  Piece& turn_piece = turn_player.piece_[piece_index];*/
  const int index = CalcIndex(player_index, piece_index);
  int& current_pos = pos_[index];

  if (current_pos == 0) {
    if(count != 5) return Movement::NoMoves;
    if (CheckBoxBridge(EntryBox(player_index))) return Movement::NoMoves;
    current_pos = EntryBox(player_index);
    return Movement::Normal;
  }

  if (InRunway(player_index, current_pos)) {
    return ManageRunway(piece_index, player_index, count);
  }

  int target = current_pos + count;

  int steps = count;
  int piece_pos = current_pos;
  for (int s = 0; s < steps; ++s) {
    piece_pos = (piece_pos < kBoxNum) ? piece_pos + 1 : 1;
    if (CheckBoxBridge(piece_pos)) return Movement::NoMoves;
  }


  if (target == ExitBox(player_index)) {
    current_pos = ExitBox(player_index) + 1;
    return Movement::ReachExit;
  }
  
  bool is_safe = IsBoxSafe(target);
  bool did_eat = false;

  if (!is_safe && CountPiecesOnBox(target) == 1) {
    for (int i = 0; i < kNumPlayers; i++)
    {
      if (i == player_index) continue;
      for (int j = 0; j < kPiecesPerPlayer; j++)
      {
        int other_index = CalcIndex(i, j);
        if (pos_[other_index] == target) {
          pos_[other_index] = 0;
          did_eat = true;
          break;
        }
      }
    }
  }
  current_pos = target;
  return did_eat ? Movement::Eat : Movement::Normal;
}

MGMParcheesi* MGMParcheesi::Clone() const{
  return new MGMParcheesi{ *this };
}