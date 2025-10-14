#include "deck.h"
#include "player.h"

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

  BlackJack(int num_players);

  void DealHand();
  TableState Bets();
  void NextPlayer();
  HandResult HandResult();
};
