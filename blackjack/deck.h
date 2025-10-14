#pragma once
#ifndef DECK_H
#define DECK_H

enum Suit{
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

struct Deck{
  static const int kCardPerSuit = 13;
  static const int kSuitCount = 4;
  static const int kCardTotal = kCardPerSuit * kSuitCount;
  
  int deck[kCardTotal];
  CardType card_type;
  Suit card_suit;

  Deck();
};

#endif