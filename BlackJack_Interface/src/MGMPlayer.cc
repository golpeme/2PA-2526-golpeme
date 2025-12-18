#pragma once
#include "MGMPlayer.h"


MGMPlayer::MGMPlayer(BaseRules& rules) : rules_(rules) {}

MGMPlayer::Decision MGMPlayer::GetMatCorrectDecision(HandInfo info, ITable::Card dealer_card){
  int row_idx = -1;
  int col_idx = GetCardValue(dealer_card) - 2;
  //mat_correct_behaivour[own_cards][dealer_cards]
  /*if(is_pair) filas 16-25
  if(is_soft) filas 10-15
  else filas 0-9*/
  if (info.is_pair){
    row_idx = PairRowIdx(info.total / 2);
    printf("\nis pair");
  }
  else if(info.is_soft){
    row_idx = SoftRowIdx(info.total);
    printf("\nis soft");
  }else{
    row_idx = HardRowIdx(info.total); 
    printf("\nis hard");
  }

  return mat_correct_behaivour_[row_idx][col_idx];
}

int MGMPlayer::HardRowIdx(int total){
  if(total <= 8) return 0;
  if(total >= 17) return 9;

  return total - 8;
}

int MGMPlayer::SoftRowIdx(int total){
  //18 -> row_idx = 10
  //19 -> 11...
  //total + row = 28

  return 28 - total;
}

int MGMPlayer::PairRowIdx(int pair_rank) {
  switch (pair_rank)
  {
  case 11: return 16;
    
  case 9: return 17;
    
  case 8: return 18;
    
  case 7: return 19;
    
  case 6: return 20;
    
  case 5: return 21;
    
  case 4: return 22;
    
  case 3: return 23;
    
  case 2: return 24;
    
  case 10: return 25;
    
  default: return -1;
    
  }
}

MGMPlayer::HandInfo MGMPlayer::HandData(const ITable::Hand& hand){
  HandInfo info{};
  std::vector<ITable::Value> values;

  for (const ITable::Card& card : hand)
  {
    values.push_back(card.value_);
    info.total += GetCardValue(card);
  }
  
  if (values[0] == values[1]) info.is_pair = true;
  else if ((values[0] == ITable::Value::ACE || values[1] == ITable::Value::ACE)
    && info.total > 21) {
    info.total -= 10;
    info.is_soft = false;
  }
  else if (values[0] == ITable::Value::ACE || values[1] == ITable::Value::ACE
    && info.total <= 21) info.is_soft = true;
  else info.is_soft = false;
  return info;
}

/*if(is_pair) filas 16-25
if(is_soft) filas 10-15
else filas 0-9*/

int MGMPlayer::GetCardValue(ITable::Card card){

    ITable::Value value = card.value_;
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
  
  void MGMPlayer::SetRandomBehaivour(){
    behaivour_ = static_cast<PlayerBehaivour>((rand()%4));
  }
  /*if(is_pair) filas 16-25
if(is_soft) filas 10-15
else filas 0-9*/
ITable::Action MGMPlayer::DecidePlayerAction(const ITable& table, int player_index, int hand_index){
  switch (behaivour_)
  {
  case kPB_MatCorrect: {
    HandInfo info{};
    ITable::Hand current_hand = table.GetHand(player_index, hand_index);
    info = HandData(current_hand);
    Decision player_dec = GetMatCorrectDecision(info, table.GetDealerCard());
    
    switch (player_dec)
    {
    case Decision::Stand: return ITable::Action::Stand;
      
        case Decision::Hit: return ITable::Action::Hit;
      
        case Decision::Split: return ITable::Action::Split;
      
        case Decision::Double: return ITable::Action::Double;
      
    default:
      return ITable::Action::Stand;
    }
    return ITable::Action::Stand;
  }
    
  case kPB_DrunkPlayer:
    return static_cast<ITable::Action>((rand()%4));
    
  case kPB_FearOfSuccess:
    return ITable::Action::Stand;
    
  case kPB_DegenerateGambler:
    return ITable::Action::Hit;
    
  default:
    return ITable::Action::Stand;
    break;
  }
}

int MGMPlayer::DecideInitialBet(const ITable& table, int player_index){
  BaseRules rules;
  int current_money = table.GetPlayerMoney(player_index);
  int bet = 0;
  int max_bet = rules.MaximumInitialBet();
  int min_bet = rules.MinimumInitialBet();
  switch (behaivour_)
  {
  case kPB_MatCorrect:
    bet = current_money / 4;
    if (bet >= max_bet) bet = max_bet;
    if (bet <= min_bet) bet = min_bet;
    if (bet > current_money) bet = current_money;
    break;
  case kPB_DrunkPlayer:
    if (current_money > 0)
    {
      bet = ((rand() % current_money + 1));
    }
    else {
      bet = 0;
    }
    break;
  case kPB_FearOfSuccess:

    bet = min_bet;
    if (bet > current_money) bet = current_money;
    break;
  case kPB_DegenerateGambler:

    bet = current_money / 2;
    if (bet >= max_bet) bet = max_bet;
    if (bet <= min_bet) bet = min_bet;
    if (bet > current_money) bet = current_money;

    break;
  default:

    bet = current_money / 4;

    break;
  }
  return bet;
}

bool MGMPlayer::DecideUseSafe(const ITable& table, int player_index){
  switch (behaivour_)
  {
  case kPB_MatCorrect:
      return false;
    break;
  case kPB_DrunkPlayer:
    return static_cast<bool>((rand()%1));
    break;
  case kPB_FearOfSuccess:
    return false;
    break;
  case kPB_DegenerateGambler:
    return true;
    break;
  default:
      return true;
    break;
  }
}