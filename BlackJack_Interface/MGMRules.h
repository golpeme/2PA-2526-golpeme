#pragma once
#include "Rules.h"

class DisruptiveRules : public BaseRules {
public:
  ~DisruptiveRules() = default;

  int GetWinPoint() const { return 25; }

  int NumberOfDecks() const { return 2; }

  virtual int InitialCards() const { return 3; }

  virtual int InitialPlayerMoney() const { return 4000; }

  virtual int InitialDealerMoney() const { return 100000; }

  virtual int MinimumInitialBet() const { return 100; }

  virtual int MaximumInitialBet() const { return 10000; }

  virtual int DealerStop() const { return 21; }
};

class FitfluencerRules : public BaseRules {
public:
  ~FitfluencerRules() = default;

  int GetWinPoint() const { return 20; }

  int NumberOfDecks() const { return 1; }

  virtual int InitialCards() const { return 2; }

  virtual int InitialPlayerMoney() const { return 4000; }

  virtual int InitialDealerMoney() const { return 100000; }

  virtual int MinimumInitialBet() const { return 100; }

  virtual int MaximumInitialBet() const { return 10000; }

  virtual int DealerStop() const { return 16; }
};