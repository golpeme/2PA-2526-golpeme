#include "MGMplayer.h"


static int MovePriority(IParcheesi::Movement movement) {
  switch (movement)
  {
  case IParcheesi::Movement::End:
    return 100;
    break;
  case IParcheesi::Movement::Eat:
    return 90;
    break;
  case IParcheesi::Movement::ReachExit:
    return 80;
    break;
  case IParcheesi::Movement::Normal:
    return 10;
    break;
  case IParcheesi::Movement::NoMoves:
    return -10;
    break;
  case IParcheesi::Movement::IllegalPass:
  case IParcheesi::Movement::IllegalBridge:
  case IParcheesi::Movement::IllegalBoxFull:
  case IParcheesi::Movement::IllegalPastEnd:
  case IParcheesi::Movement::IllegalEntryBlocked:
  case IParcheesi::Movement::IllegalPieceAtHome:
  case IParcheesi::Movement::IllegalMustEnterPiece:
  case IParcheesi::Movement::IllegalMustBreakBridge:
    return -1000;
  default:
    return -1000;
    break;
  }
}

MGMPlayer::MGMPlayer(int player_index) : entry_point(player_index) {}

int MGMPlayer::DecideMove(const IParcheesi &parcheesi, int player_index, int dice_roll) const {
  int best_piece = 0;
  int best_score = -10000;
  int piece_without_moves = -1;
  bool found_legal = false;

  for (int i = 0; i < kPieceNum; i++)
  {
    IParcheesi* tmp = parcheesi.Clone();
    IParcheesi::Movement result = tmp->ApplyMovement(i, player_index, dice_roll);
    int score = MovePriority(result);
    delete tmp;

    if (result == IParcheesi::Movement::NoMoves && piece_without_moves == -1) {
      piece_without_moves = i;
    }

    bool is_legal = (result == IParcheesi::Movement::End
      || result == IParcheesi::Movement::Eat
      || result == IParcheesi::Movement::ReachExit
      || result == IParcheesi::Movement::Normal);

    if (is_legal)
    {
      if (score > best_score || (score == best_score && i > best_piece))
      {
        found_legal = true;
        best_score = score;
        best_piece = i;
      }
    }
    else if (!found_legal) {
      if (score > best_score || (score == best_score && i > best_piece)) {
        best_score = score;
        best_piece = i;
      }
    }
  }
  if (!found_legal && piece_without_moves != -1) {
    return piece_without_moves;
  }
  return best_piece;
}
//const int Player::kPieceNum = 4;

/*Player::Player(Colors color)
  : color_{color}
  , piece_{0,0,0,0} {
}

bool Player::PiecesAtHome(){
  if(CountPiecesOnBox(0) != 0){
    return true;
  }
  return false;
}

int Player::CountPiecesOnBox(int box){
  int occupants = 0;
  for(int i = 0; i < kPieceNum; i++){
    if(piece_[i].position == box){
      occupants++;
    }
  }
  return occupants;
}*/
