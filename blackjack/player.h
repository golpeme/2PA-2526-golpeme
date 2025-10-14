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

  int drawn_card;
  Suit Deck::card_suit;
  Suit drawn_card_suit;
  CardType drawn_card_type;

  PlayerAction DecideAction(PlayerBehaivour behaivour);
};
