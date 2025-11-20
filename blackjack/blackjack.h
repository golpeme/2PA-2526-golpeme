#pragma once
#include "deck.h"
#include "player.h"
#include "crupier.h"

enum TableState{
  kTS_MakeBets = 0,
  kTS_CrupierDealing,
  kTS_CrupierStands,
  kTS_PlayerDouble,
  kTS_PlayerSplit,
  kTS_PlayerHit,
  kTS_PlayerStands,
  kTS_SafeBet,
  kTS_End
};

enum HandResult{
  kHR_PlayerWins = 0,
  kHR_CrupierWins,
  kHR_NoWin,
  kHR_Tie,
  kHR_SafeBetHits,
  kHR_End
};

struct BlackJack
{
  static const int kMaxPlayers = 3;
  static const int initial_budget = 500;
  static const int max_bet = 300;

  Crupier dealer;
  Player players[kMaxPlayers];
  int num_players = 1;

  CardList dealer_hand;
  int current_player = 0;

  bool first_hand = true;

  BlackJack(int num_players);

  void StartRound();
  void PlayerPhase(int player_idx);
  void DealerPhase();
  void Settle();
  
  TableState Bets();
  HandResult HandResult();
  void NextPlayer();

  static int HandValue(const CardList& hand);
  static bool IsBlackJack(const CardList& hand);
};


