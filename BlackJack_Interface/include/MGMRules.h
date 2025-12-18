#pragma once
#include "Rules.h"

/// @class DisruptiveRules
/// @brief Advanced ruleset with elevated difficulty and winning threshold of 25.
/// 
/// Implements challenging gameplay conditions including 2-deck shoe,
/// 3 initial cards per player, and dealer standing at 21.
/// Provides higher risk and reward dynamic compared to standard rules.
class DisruptiveRules : public BaseRules {
public:
  ~DisruptiveRules() = default;

  /// @brief Gets the target winning point.
  /// @return 25 points required to win in this ruleset
  int GetWinPoint() const { return 25; }

  /// @brief Gets the number of decks used.
  /// @return 2 decks in the shoe
  int NumberOfDecks() const { return 2; }

  /// @brief Gets the number of initial cards dealt to each player.
  /// @return 3 cards per player at round start
  virtual int InitialCards() const { return 3; }

  /// @brief Gets the starting money for players.
  /// @return 4000 currency units per player
  virtual int InitialPlayerMoney() const { return 4000; }

  /// @brief Gets the starting money for the dealer.
  /// @return 100000 currency units for the dealer
  virtual int InitialDealerMoney() const { return 100000; }

  /// @brief Gets the minimum allowed bet.
  /// @return 100 currency units minimum bet
  virtual int MinimumInitialBet() const { return 100; }

  /// @brief Gets the maximum allowed bet.
  /// @return 10000 currency units maximum bet
  virtual int MaximumInitialBet() const { return 10000; }

  /// @brief Gets the value at which the dealer must stand.
  /// @return 21 points (dealer must stand at or above)
  virtual int DealerStop() const { return 21; }
};

/// @class FitfluencerRules
/// @brief Standard ruleset with reduced winning threshold of 20.
/// 
/// Classic single-deck blackjack with traditional 2 initial cards
/// and soft 17 rule (dealer hits on soft 17, stands on hard 17+).
/// Most closely resembles standard casino blackjack rules.
class FitfluencerRules : public BaseRules {
public:
  ~FitfluencerRules() = default;

  /// @brief Gets the target winning point.
  /// @return 20 points required to win in this ruleset
  int GetWinPoint() const { return 20; }

  /// @brief Gets the number of decks used.
  /// @return 1 deck
  int NumberOfDecks() const { return 1; }

  /// @brief Gets the number of initial cards dealt to each player.
  /// @return 2 cards per player at round start
  virtual int InitialCards() const { return 2; }

  /// @brief Gets the starting money for players.
  /// @return 4000 currency units per player
  virtual int InitialPlayerMoney() const { return 4000; }

  /// @brief Gets the starting money for the dealer.
  /// @return 100000 currency units for the dealer
  virtual int InitialDealerMoney() const { return 100000; }

  /// @brief Gets the minimum allowed bet.
  /// @return 100 currency units minimum bet
  virtual int MinimumInitialBet() const { return 100; }

  /// @brief Gets the maximum allowed bet.
  /// @return 10000 currency units maximum bet
  virtual int MaximumInitialBet() const { return 10000; }

  /// @brief Gets the value at which the dealer must stand.
  /// @return 16 points (soft 17 rule: dealer hits on soft 17)
  virtual int DealerStop() const { return 16; }
};
