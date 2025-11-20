#include "Rules.h"

class MGMRules : BaseRules{
  public:
  //~BaseRules() = default;
  int GetWinPoint() const { return 21; }
  int NumberOfDecks() const { return 1; }
  int InitialCards() const { return 2; }
  int InitialPlayerMoney() const { return 4000; }
  int InitialDealerMoney() const { return 100000; }
  int MinimumInitialBet() const { return 100; }
  int MaximumInitialBet() const { return 10000; }
  int DealerStop() const { return 17; }
};