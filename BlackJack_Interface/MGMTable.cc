#pragma once
#include "MGMTable.h"
#include <algorithm>
#include <random>

MGMTable::MGMTable(int num_players, const BaseRules& rules)
    : rules_{ rules },
      num_players_{ num_players },
      dealer_money_{ rules.InitialDealerMoney() }
{
    total_player_money_.resize(num_players_);
    player_bets_.resize(num_players_);
    hands_.resize(num_players_);
    players_.resize(num_players_, nullptr);

    for (int i = 0; i < num_players_; i++)
        total_player_money_[i] = rules.InitialPlayerMoney();
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
            for (int v = 0; v < kValueNum; v++)
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
    if(total_player_money_[player_index] < money ||
    money < rules_.MinimumInitialBet() || money > rules_.MaximumInitialBet()){
        return ITable::Result::Illegal;
    }else{
        return ITable::Result::Ok;
    }
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
  std::vector<Value> values;

  for (const Card& card : hand)
  {
    values.push_back(card.value_);
    info.total += GetCardValue(card);
  }
  
  if(values[0] == values[1]) info.is_pair = true;
  if((values[0] == Value::ACE || values[1] == Value::ACE)
     && info.total > 21){
      info.total -= 10;
      info.is_soft = false;
     } else info.is_soft = true;

  return info;
}

MGMTable::Result MGMTable::ApplyPlayerAction(int player_index, int hand_index, Action action) {
    Hand checked_hand = GetHand(player_index, hand_index);
    HandInfo hand_info = HandData(checked_hand);

    if (hand_info.total > rules_.GetWinPoint() && !hand_info.is_soft)
    {
        return Result::Illegal;
    }

    if(player_bets_[player_index][hand_index] > rules_.MaximumInitialBet() ||
    player_bets_[player_index][hand_index] < rules_.MinimumInitialBet() ||
    total_player_money_[player_index] > player_bets_[player_index][hand_index])
    {
        return Result::Illegal;
    }

    switch (action)
    {
    case Action::Stand:
        return Result::Ok;
        break;

    case Action::Hit:
        
        break;

    case Action::Double:
        player_bets_[player_index][hand_index] = GetPlayerCurrentBet(player_index, hand_index) * 2;
        
        break;

    case Action::Split:
        hands_[player_index][hand_index][0] = checked_hand[0];
        hands_[player_index][hand_index + 1][1] = checked_hand[1];
        player_bets_[player_index][hand_index + 1] = player_bets_[player_index][hand_index];
        break;

    default:
        break;
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
    int dealer_value = 0;
    std::vector<int> hand_num;
    for (int i = 0; i < player_num_; i++)
    {
        hand_num[i] = GetNumberOfHands(i);
    }
    

    std::vector<int> hand_values;
    RoundEndInfo end_info{};
    for(const Card& card : end_info.dealer_hand){
        dealer_value += GetCardValue(card);
    }
    for (int i = 0; i < player_num_; i++)
    {   
        for (int j = 0; j < hand_values.size(); j++)
        {   
            for(const Card& card : hands_[i][j]){
                hand_values[j] += GetCardValue(card);
            }
        }
    }

    for (int i = 0; i < player_num_; i++)
    {   
        if(dealer_value > hand_values[i] && dealer_value <= rules_.GetWinPoint()){
            //end_info.winners[i] = ITable::RoundEndInfo::BetResult::Lose;
        }
    }
    return end_info;
}

int MGMTable::GetPlayerInitialBet(int player_index) const {
    return player_bets_[player_index][0];
}

void MGMTable::CleanTable() {
  for (auto& hand : hands_)
  {
    hands_.clear();
    hands_.resize(1);
  }
  for (auto& bets : player_bets_)
  {
    player_bets_.clear();
    player_bets_.resize(1);
  }
    deck_.clear();
    dealer_hand_.clear();
}