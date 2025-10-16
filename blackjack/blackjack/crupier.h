#include "deck.h"
#include <time.h>
#include <stdlib.h>


enum CrupierAction{
  kCA_Deal = 0,
  kCA_SelfDeal,
  kCA_Stand,
  kCA_End
};

struct Crupier
{
  static const int stand_value = 16;

  Card Deck::drawn_card;

  CardType CardTypeInfoOutPut();
  Card DrawCard();
  CrupierAction DecideAction();
};

Crupier dealer;

CardType CardTypeInfoOutput(){
  int face_card_checker = rand() % 4;

  switch (face_card_checker)
  {
    case 0:
      return kNumbered_Card; 
      break;
    case 1:
      return kFaceCard_Jack;
      break;
    case 2:
      return kFaceCard_Queen;
      break;
    case 3:
      return kFaceCard_King;
      break;
    default:
      return kNumbered_Card; 
      break;
  }  
}

Card DrawCard(){
  dealer.drawn_card.card_value = rand() % 13 + 1;
  dealer.drawn_card.card_suit = static_cast<CardSuit> (rand() % 4 + 1);
  if (dealer.drawn_card.card_value == 1 || dealer.drawn_card.card_value == 11)
  {
    dealer.drawn_card.card_type = kCard_Ace;
  }else if(dealer.drawn_card.card_value == 10){
    dealer.drawn_card.card_type = CardTypeInfoOutput();
  }else {
    dealer.drawn_card.card_type = kNumbered_Card;
  }
  return dealer.drawn_card;
}


