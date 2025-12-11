#pragma once
#include "MGMTable.h"
#include <algorithm>
#include <random>

MGMTable::MGMTable(int num_players, const BaseRules& rules)
    : rules_{ rules },
      player_num_{ num_players },
      dealer_money_{ rules.InitialDealerMoney() }
{
    total_player_money_.resize(player_num_);
    player_bets_.resize(player_num_);
    hands_.resize(player_num_);
    players_.resize(player_num_, nullptr);

    for (int i = 0; i < player_num_; i++) {
      total_player_money_[i] = rules.InitialPlayerMoney();

      hands_[i].resize(1);

      player_bets_[i].resize(1, 0);
    }
}

int MGMTable::GetCardValue(Card card){
    Value value = card.value_;

    switch (value)
    {
      case ITable::Value::ACE: return 11;
      
      case ITable::Value::TWO: return 2;
      
      case ITable::Value::THREE: return 3;
      
      case ITable::Value::FOUR: return 4;
      
      case ITable::Value::FIVE: return 5;
      
      case ITable::Value::SIX: return 6;
      
      case ITable::Value::SEVEN: return 7;
      
      case ITable::Value::EIGHT: return 8;
      break;
      
      case ITable::Value::NINE: return 9;
      break;
      
      case ITable::Value::TEN: return 10;
      break;
      
      case ITable::Value::JACK: return 10;
      break;
      
      case ITable::Value::QUEEN: return 10;
      break;
      
      case ITable::Value::KING: return 10;
      break;
      
      default: return 0;
      break;
    }
}

void MGMTable::FillDeck() {
    for (int i = 0; i < rules_.NumberOfDecks(); i++)
    {
        for (int s = 0; s < kSuitNum; s++)
        {
            for (int v = 1; v <= kValueNum; v++)
            {
                Card tmp = { static_cast<Value>(v), static_cast<Suit>(s) };
                deck_.push_back(tmp);
            }
        }
    }
}

void MGMTable::ShuffleDeck() {
    std::random_device seed;
    std::mt19937 rand(seed());

    for (int i = 0; i < rules_.NumberOfDecks(); i++) {
        std::shuffle(deck_.begin(), deck_.end(), rand);
    }
}

MGMTable::Hand MGMTable::GetHand(int player_index, int hand_index) const {
    return hands_[player_index][hand_index];
}

int MGMTable::GetNumberOfHands(int player_index) const {
    return static_cast<int>(hands_[player_index].size());
}

int MGMTable::GetPlayerMoney(int player_index) const {
    return total_player_money_[player_index];
}

int MGMTable::GetPlayerCurrentBet(int player_index, int hand_index) const {
    return player_bets_[player_index][hand_index];
}

MGMTable::Result MGMTable::PlayInitialBet(int player_index, int money) {
  if (money < rules_.MinimumInitialBet() || money > rules_.MaximumInitialBet()) {
    return ITable::Result::Illegal;
  }

  if (total_player_money_[player_index] < money) {
    return ITable::Result::Illegal;
  }

  total_player_money_[player_index] -= money;
  player_bets_[player_index][0] = money;



  return ITable::Result::Ok;
}

MGMTable::Card MGMTable::GetDealerCard() const {
    return dealer_hand_.back();
}

int MGMTable::DealerMoney() const {
    return dealer_money_;
}

// Dealer es parte de la mesa, no tiene elecciones propias mas alla de aplicar las reglas

void MGMTable::DealCard(int player_index, int hand_index) {
    Card dealt_card = deck_.back();
    deck_.pop_back();

    hands_[player_index][hand_index].push_back(dealt_card);
}

MGMTable::Result MGMTable::PlaySafeBet(int player_index) {//half of bet
    Card dealer_card = GetDealerCard();
    int player_money = GetPlayerMoney(player_index);
    if(dealer_card.value_ == Value::ACE && player_money >= rules_.MinimumInitialBet()){
        int safe_bet = (GetPlayerInitialBet(player_index) / 2);
        
        total_player_money_[player_index] -= safe_bet;
        dealer_money_ += safe_bet;
        
        return Result::Ok;
    }else return Result::Illegal;
}

MGMTable::HandInfo MGMTable::HandData(const Hand& hand){
  HandInfo info{};

  info.is_pair = false;
  info.is_soft = false;
  info.total = 0;

  int ace_count = 0;

  for (const Card& card : hand)
  {
    info.total += GetCardValue(card);
    if (card.value_ == ITable::Value::ACE) {
      ace_count++;
    }
  }
    
    while (info.total > rules_.GetWinPoint() && ace_count > 0) {
      info.total -= 10;
      ace_count--;
    }

    info.is_soft = (ace_count > 0 && info.total <= rules_.GetWinPoint());

    if (hand.size() == 2)
    {
      info.is_pair = hand[0].value_ == hand[1].value_;
    }

  return info;
}

MGMTable::Result MGMTable::ApplyPlayerAction(int player_index, int hand_index, Action action) {
    Hand checked_hand = GetHand(player_index, hand_index);
    HandInfo hand_info = HandData(checked_hand);

    if (hand_info.total > rules_.GetWinPoint())
    {
        return Result::Illegal;
    }

    if(player_bets_[player_index][hand_index] > rules_.MaximumInitialBet() ||
    player_bets_[player_index][hand_index] < rules_.MinimumInitialBet() ||
    total_player_money_[player_index] > player_bets_[player_index][hand_index])
    {
        return Result::Illegal;
    }

    int current_bet = player_bets_[player_index][hand_index];
    Hand new_hand;

    switch (action)
    {
    case Action::Stand:
        return Result::Ok;
        break;

    case Action::Hit:
      DealCard(player_index, hand_index);
      return Result::Ok;
        break;

    case Action::Double:


        total_player_money_[player_index] -= current_bet;
        player_bets_[player_index][hand_index] = GetPlayerCurrentBet(player_index, hand_index) * 2;

        DealCard(player_index, hand_index);
        return Result::Ok;
        break;

    case Action::Split:
      //resize hand and bet size
      if (checked_hand.size() != 2) {
        return Result::Illegal;
      }
      if (!hand_info.is_pair) {
        return Result::Illegal;
      }

      if (total_player_money_[player_index] < current_bet) {
        return Result::Illegal;
      }

      total_player_money_[player_index] -= current_bet;

      new_hand.push_back(checked_hand[1]);
      hands_[player_index].push_back(new_hand);

      hands_[player_index][hand_index].pop_back();

      player_bets_[player_index].push_back(current_bet);

      DealCard(player_index, hand_index);
      DealCard(player_index, (static_cast<int>(hands_[player_index].size()) - 1));

      return Result::Ok;
        break;

    default:
      return Result::Illegal;
    }
    return Result::Ok;
}



void MGMTable::StartRound() {
    
    if(dealer_hand_.size() > 0){
        dealer_hand_.clear();
    }
    
    FillDeck();
    ShuffleDeck();
    
    Card dealer_c = deck_.back();
    dealer_hand_.push_back(dealer_c);
    deck_.pop_back();
}

MGMTable::RoundEndInfo MGMTable::FinishRound() {
  RoundEndInfo end_info{};

  //count hands
  std::vector<int> hand_num(player_num_);
  for (int i = 0; i < player_num_; i++) {
    hand_num[i] = GetNumberOfHands(i);
  }

  //calc dealer value
  int dealer_value = 0;
  for (const Card& card : dealer_hand_) {
    dealer_value += GetCardValue(card);
  }

  //adjust dealer aces
  int dealer_aces = 0;
  for (const Card& card : dealer_hand_) {
    if (card.value_ == Value::ACE) dealer_aces++;
  }
  while (dealer_value > rules_.GetWinPoint() && dealer_aces > 0) {
    dealer_value -= 10;
    dealer_aces--;
  }

  //dealer deals
  while (dealer_value < rules_.DealerStop()) {
    Card new_card = deck_.back();
    deck_.pop_back();
    dealer_hand_.push_back(new_card);

    int card_value = GetCardValue(new_card);
    dealer_value += card_value;

    if (new_card.value_ == Value::ACE) dealer_aces++;

    //adjust for aces if soft
    while (dealer_value > rules_.GetWinPoint() && dealer_aces > 0) {
      dealer_value -= 10;
      dealer_aces--;
    }
  }

  printf("\n[DEBUG] Dealer final value: %d\n", dealer_value);

  //init result structs
  end_info.winners.resize(player_num_);
  end_info.player_money_delta.resize(player_num_, 0);
  end_info.dealer_money_delta = 0;
  end_info.dealer_hand = dealer_hand_;

  for (int i = 0; i < player_num_; i++) {
    end_info.winners[i].resize(hand_num[i]);

    for (int j = 0; j < hand_num[i]; j++) {
      HandInfo player_info = HandData(hands_[i][j]);
      int player_value = player_info.total;
      int bet = player_bets_[i][j];

      printf("[DEBUG] Player %d Hand %d: value=%d, bet=%d\n", i, j, player_value, bet);

      //get results
      bool player_bust = (player_value > rules_.GetWinPoint());
      bool dealer_bust = (dealer_value > rules_.GetWinPoint());

      if (player_bust) {

        end_info.winners[i][j] = RoundEndInfo::BetResult::Lose;
        end_info.player_money_delta[i] += 0;
        end_info.dealer_money_delta += bet; 
        printf("[DEBUG] -> Player BUST, loses %d\n", bet);
      }
      else if (dealer_bust) {

        end_info.winners[i][j] = RoundEndInfo::BetResult::Win;
        end_info.player_money_delta[i] += bet * 2;
        end_info.dealer_money_delta -= bet;
        printf("[DEBUG] -> Dealer BUST, player wins %d\n", bet);
      }
      else if (player_value > dealer_value) {

        end_info.winners[i][j] = RoundEndInfo::BetResult::Win;
        end_info.player_money_delta[i] += bet * 2;
        end_info.dealer_money_delta -= bet;
        printf("[DEBUG] -> Player WINS by points, wins %d\n", bet);
      }
      else if (dealer_value > player_value) {

        end_info.winners[i][j] = RoundEndInfo::BetResult::Lose;
        end_info.player_money_delta[i] += 0;
        end_info.dealer_money_delta += bet;
        printf("[DEBUG] -> Dealer WINS by points, player loses %d\n", bet);
      }
      else {

        end_info.winners[i][j] = RoundEndInfo::BetResult::Tie;
        end_info.player_money_delta[i] += bet;  
        end_info.dealer_money_delta += 0;    
        printf("[DEBUG] -> TIE, player gets back %d\n", bet);
      }
    }
  }

  for (int i = 0; i < player_num_; i++) {
    total_player_money_[i] += end_info.player_money_delta[i];
    printf("[DEBUG] Player %d final money: %d (delta: %d)\n",
      i, total_player_money_[i], end_info.player_money_delta[i]);
  }

  dealer_money_ += end_info.dealer_money_delta;
  printf("[DEBUG] Dealer final money: %d (delta: %d)\n",
    dealer_money_, end_info.dealer_money_delta);

  return end_info;
}

int MGMTable::GetPlayerInitialBet(int player_index) const {
    return player_bets_[player_index][0];
}

void MGMTable::RemovePlayers(int player_index) {
  players_.erase(players_.begin() + player_index);
  player_bets_.erase(player_bets_.begin() + player_index);
  total_player_money_.erase(total_player_money_.begin() + player_index);
  hands_.erase(hands_.begin() + player_index);
  player_num_--;
}



void MGMTable::CleanTable() {
  for (int i = 0; i < player_num_; i++) {
    for (auto& player_hand : hands_[i]) {
      player_hand.clear();
    }
    hands_[i].resize(1);


    for (auto& bet : player_bets_[i]) {
      bet = 0;
    }
    player_bets_[i].resize(1, 0);
  }

  for (int i = player_num_ - 1; i >= 0; i--) {
    if (total_player_money_[i] <= rules_.MinimumInitialBet()) {
      printf("\nPlayer %d has been eliminated (money: %d)\n",
        i, total_player_money_[i]);
      RemovePlayers(i);
    }
  }

  deck_.clear();
  dealer_hand_.clear();
}


void MGMTable::DealerRevealSecondCard() {
  dealer_hand_.push_back(deck_.back());
  deck_.pop_back();
}

MGMTable::Hand MGMTable::GetDealerHand() {
  return dealer_hand_;
}