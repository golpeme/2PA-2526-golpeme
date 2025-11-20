#include "blackjack.h"
#include <time.h>
#include <stdlib.h>

int BlackJack::HandValue(const CardList& hand){
  int total = 0;
  int aces = 0;

  for(Node* n = hand.head; n; n = n->next){
    total += n->data.card_value;
    if(n->data.card_type == kCard_Ace) aces++;
  }
  while(total > 21 && aces > 0){
    total -= 10;
    aces--;
  }
  return total;
}

static bool CardListIsTwoCards(const CardList& hand){
  if(hand.size == 2) return true;
  else return false;
}

bool BlackJack::IsBlackJack(const CardList& hand){
  if(HandValue(hand) == 21) return true;
  else return false;
}

BlackJack::BlackJack(int num) : num_players(num){
  if(num_players < 1) num_players = 1;
  if(num_players > kMaxPlayers) num_players = kMaxPlayers;

  for (int i = 0; i < num_players; i++)
  {
    PlayerBehaivour bhv;
    bhv = static_cast<PlayerBehaivour> (rand() % 4);
    players[i].Init(bhv, initial_budget, 50);
  }
  CardListInit(&dealer_hand);
}

void BlackJack::StartRound(){
  CardListFree(&dealer_hand);
  CardListInit(&dealer_hand);
  dealer.ResetAndShuffle();

  for (int i = 0; i < num_players; i++)
  {
    players[i].NewRound();
  }
  
  for (int i = 0; i < num_players; i++) PlayerPhase(i);

  DealerPhase();

  Settle();
}

void BlackJack::PlayerPhase(int idx){
  Card dealer_up = dealer_hand.head->data;

  for (int i = 0; i <players[idx].hand_count; i++)
  {
    Hand& hand = players[idx].hands[i];
    
    if(hand.IsBlackjack()){
      hand.finished = true;
      continue;
    }

    while(!hand.finished && !hand.IsBust()){
      PlayerAction action = players[idx].DecideAction(hand, dealer_up);

      if(action == kPA_Split && players[idx].TrySplit(i)){
        players[idx].RecieveCard(i, dealer.DealCards());
        players[idx].RecieveCard(players[idx].hand_count - 1, dealer.DealCards());
      }else if(action == kPA_Double && players[idx].CanDouble(hand)){
        players[idx].hands[i].bet *= 2;
        players[idx].RecieveCard(i, dealer.DealCards());
        players[idx].FinishHand(i);
      }else if(action == kPA_Hit){
        players[idx].RecieveCard(i, dealer.DealCards());
      }else if(action == kPA_Stand){
        players[idx].FinishHand(i);
      }
    }
  }
}

void BlackJack::DealerPhase(){
  int dealer_value = HandValue(dealer_hand);
  while (dealer.DecideAction(dealer_value) == kCA_SelfDeal) {
    AddCardToList(&dealer_hand, dealer.DealCards());
    dealer_value = HandValue(dealer_hand);
  }
}

void BlackJack::Settle(){
  const int dealer_value = HandValue(dealer_hand);
  printf("Dealer value: %d\n", dealer_value);

  for (int i = 0; i < num_players; i++)
  {
    for (int j = 0; j < players[i].hand_count; i++)
    {
      const Hand& player_hand = players[i].hands[j];
      const int player_value = player_hand.Value();

      const char* result = "Tie";
      if(player_hand.IsBlackjack() && dealer_value != 21) result = "Player BlackJack";
      else if(player_value > 21) result = "Player Bust -> Dealer Wins";
      else if(dealer_value > 21) result = "Dealer Bust -> Player Wins";
      else if(player_value > dealer_value) result = "Player Wins";
      else if(player_value < dealer_value) result = "Dealer Wins";
      else result = "Tie";

      printf("Player %d Hand %d -> %d vs %d : %s\n", i, j, player_value, dealer_value, result);
    }
  }
}

TableState BlackJack::Bets() {
  return kTS_MakeBets;
}

HandResult BlackJack::HandResult() {
  return kHR_NoWin;
}

void BlackJack::NextPlayer() {
  if (++current_player >= num_players) current_player = 0;
}
