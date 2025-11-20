#include "crupier.h"
#include "deck.h"

enum PlayerAction{
  kPA_Stand = 0,
  kPA_Double,
  kPA_Hit,
  kPA_Split,
  kPA_End
};

enum PlayerBehaivour{
  kPB_MatCorrect = 0,
  kPB_DrunkPlayer,
  kPB_FearOfSuccess,
  kPB_DegenerateGambler,
  kPB_End
};

struct Hand{
  CardList cards;
  int bet;
  bool finished;

  void Init(int initial_bet = 0);
  void Clear();
  void AddCard(Card card);
  int Value() const;
  bool IsBlackjack() const;
  bool IsBust() const;
  bool CanSplit() const;
};

struct Player
{
  PlayerAction action;
  PlayerBehaivour behaivour;

  int money;
  int base_bet;

  Hand hands[4];
  int hand_count;

  void Init(PlayerBehaivour bhv, int money_init, int base_bet_init);
  void NewRound();
  void RecieveCard(int hand_idx, Card card);
  void FinishHand(int hand_idx);

  PlayerAction DecideAction(const Hand& hand, Card dealer_card) const;
  bool TrySplit(int hand_idx);
  bool CanDouble(const Hand& hand) const;
};
