#include "deck.h"


Deck::Deck(){
  int ranks[13] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 10, 10, 10};
  for (int i = 0; i < Deck::kSuitCount; i++)
  {
    for (int j = 0; j < kCardPerSuit; j++)
    {
      Deck::deck[i] = ranks[j];
    }
  }
}