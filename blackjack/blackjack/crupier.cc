#include "deck.cc"
#include "crupier.h"

Crupier::Crupier() : next_deck_idx(0){
  ResetAndShuffle();
}

void Crupier::ResetAndShuffle(){
  current_deck = Deck();
  ShuffleDeck(current_deck.deck);
  next_deck_idx = 0;
}

Card Crupier::DrawCard(){
  if (next_deck_idx >= Deck::kCardTotal)
  {
    ShuffleDeck(current_deck.deck);
    next_deck_idx = 0;
  }
  drawn_card = current_deck.deck[next_deck_idx++];
  return drawn_card;
}

Card Crupier::DealCards(){
  return DrawCard();
}

CrupierAction Crupier::DecideAction(int current_value) const{
  return (current_value < stand_value) ? kCA_SelfDeal : kCA_Stand;
}
