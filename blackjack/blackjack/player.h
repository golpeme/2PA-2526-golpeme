#include "crupier.h"
#include "deck.h"

enum PlayerAction{
  kPA_Stand = 0,
  kPA_Double,
  kPA_Hit,
  kPA_Split,
  kPA_End
};

enum PlayerBehaivour{
  kPB_MatCorrect = 0,
  kPB_DrunkPlayer,
  kPB_FearOfSuccess,
  kPB_DegenerateGambler,
  kPB_End
};

struct Player
{
  PlayerAction action;
  PlayerBehaivour behaivour;

  Card drawn_card;

  PlayerAction DecideAction(PlayerBehaivour behaivour);
};
