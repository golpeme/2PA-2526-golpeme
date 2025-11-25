#include "MGMTable.h"

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
    int current_value = 0;

    switch (value)
    {
      case ITable::Value::ACE:
      // Ace (1 or 11)
      current_value += 11;
      break;
      
      case ITable::Value::TWO:
      // 2
      current_value += 2;
      break;
      
      case ITable::Value::THREE:
      // 3
      current_value += 3;
      break;
      
      case ITable::Value::FOUR:
      // 4
      current_value += 4;
      break;
      
      case ITable::Value::FIVE:
      // 5
      current_value += 5;
      break;
      
      case ITable::Value::SIX:
      // 6
      current_value += 6;
      break;
      
      case ITable::Value::SEVEN:
      // 7
      current_value += 7;
      break;
      
      case ITable::Value::EIGHT:
      // 8
      current_value += 8;
      break;
      
      case ITable::Value::NINE:
      // 9
      current_value += 9;
      break;
      
      case ITable::Value::TEN:
      // 10
      current_value += 10;
      break;
      
      case ITable::Value::JACK:
      // Jack (10)
      current_value += 10;
      break;
      
      case ITable::Value::QUEEN:
      // Queen (10)
      current_value += 10;
      break;
      
      case ITable::Value::KING:
      // King (10)
      current_value += 10;
      break;
      
      default:
      current_value += 0;
      break;
    }
  return current_value;
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
    return hands_[player_index].size();
}

int MGMTable::GetPlayerMoney(int player_index) const {
    return total_player_money_[player_index];
}

int MGMTable::GetPlayerCurrentBet(int player_index, int hand_index) const {
    return player_bets_[player_index][hand_index];
}

MGMTable::Result MGMTable::PlayInitialBet(int player_index, int money) {

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
    if(dealer_card.value_ == Value::ACE){
        int safe_bet = (GetPlayerInitialBet(player_index) / 2);
    }
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

}

MGMTable::RoundEndInfo MGMTable::FinishRound() {

}

int MGMTable::GetPlayerInitialBet(int player_index) const {
    return player_bets_[player_index][0];
}

void MGMTable::CleanTable() {

}