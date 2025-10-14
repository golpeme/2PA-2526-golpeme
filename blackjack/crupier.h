#include "deck.h"

enum CrupierAction{
  kCA_Deal = 0,
  kCA_SelfDeal,
  kCA_Stand,
  kCA_End
};

struct Crupier
{
  int drawn_card;
  Suit Deck::card_suit;
  Suit drawn_card_suit;
  CardType drawn_card_type;

  int DrawCard();
  CrupierAction DecideAction();
};
