#pragma once
#ifndef DECK_H
#define DECK_H

#include <stdlib.h>
#include <stdio.h>

enum CardSuit{
  kSuit_Clubs = 0,
  kSuit_Diamonds,
  kSuit_Hearts,
  kSuit_Spades,
  kSuit_End
};

enum CardType{
  kFaceCard_Jack = 0,
  kFaceCard_Queen,
  kFaceCard_King,
  kNumbered_Card,
  kCard_Ace,
  kFaceCard_End
};

struct Card{
  CardType card_type; //4 bytes
  CardSuit card_suit; //4 bytes
  int card_value;     //4 bytes
};                    //12 bytes
                      //allocating memory for the max posible cards drawn without busting
                      //would cost 12 * 11 bytes per player + 12 * 11 bytes for the dealer cards
                      //resulting in a total of 588 bytes, for now cards can be stored in an array
                      //as memory won't be a problem

struct Deck{
  static const int kCardPerSuit = 13;
  static const int kSuitCount = 4;
  static const int kCardTotal = kCardPerSuit * kSuitCount;
  
  Card deck[kCardTotal];
  CardType TypeFromRank(int rank);
  Deck();
  void ShuffleDeck(Card deck[kCardTotal]);
};



#endif