#include "deck.h"
#include <time.h>

CardType TypeFromRank(int rank){
  if (rank == 0) return kCard_Ace;
  else if (rank >= 1 && rank <= 9) return kNumbered_Card;
  else if (rank == 10) return kFaceCard_Jack;
  else if (rank == 11) return kFaceCard_Queen;
  return kFaceCard_King; // r == 12
}

Deck::Deck(){
  int idx = 0;
  for (int i = 0; i < Deck::kSuitCount; i++)
  {
    for (int j = 0; j < kCardPerSuit; j++)
    {
      Card card;
      card.card_suit = static_cast<CardSuit>(i);
      card.card_type = TypeFromRank(j);
      if(card.card_type == kCard_Ace) {
        card.card_value = 11;
      }
      else if(card.card_type == kNumbered_Card) {
        card.card_value = j + 1;
      }
      else {
        card.card_value = 10;
      }
      deck[idx++] = card;
    }
  }
}

void ShuffleDeck(Card deck[Deck::kCardTotal]){
  for (int i = Deck::kCardTotal - 1; i > 0; --i) {
    int rand_draw = rand() % (i + 1);
    Card tmp = deck[i];
    deck[i] = deck[rand_draw];
    deck[rand_draw] = tmp;
  }
}