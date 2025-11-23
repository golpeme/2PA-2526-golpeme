#include "MGMPlayer.h"

MGMPlayer::Decision MGMPlayer::GetMatCorrectDecision(HandInfo info, ITable::Card dealer_card){
  int row_idx = -1;
  int col_idx = GetCardValue(dealer_card);
  //mat_correct_behaivour[own_cards][dealer_cards]
  /*if(is_pair) filas 16-25
  if(is_soft) filas 10-15
  else filas 0-9*/
  if (info.is_pair){
    row_idx = PairRowIdx(info.total / 2);
  }
  if(info.is_soft){
    row_idx = SoftRowIdx(info.total);
  }else{
    row_idx = HardRowIdx(info.total); 
  }

  return mat_correct_behaivour_[row_idx][col_idx];
}

int MGMPlayer::HardRowIdx(int total){
  if(total <= 8) return 0;
  if(total >= 17) return 19;

  return total - 8;
}

int MGMPlayer::SoftRowIdx(int total){
  //18 -> row_idx = 10
  //19 -> 11...
  //total + row = 28
  return 28 - total;
}

int PairRowIdx(int pair_rank){
  switch (pair_rank)
  {
  case 11: return 16;
    break;
  case 9: return 17;
    break;
  case 8: return 18;
    break;
  case 7: return 19;
    break;
  case 6: return 20;
    break;
  case 5: return 21;
    break;
  case 4: return 22;
    break;
  case 3: return 23;
    break;
  case 2: return 24;
    break;
  case 10: return 25;
    break;
  default: return -1;
    break;
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
  
  if(values[0] == values[1]) info.is_pair = true;
  if((values[0] == ITable::Value::ACE || values[1] == ITable::Value::ACE)
     && info.total > 21){
      info.total -= 10;
      info.is_soft = false;
     } else info.is_soft = true;

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
      break;
        case Decision::Hit: return ITable::Action::Hit;
      break;
        case Decision::Split: return ITable::Action::Split;
      break;
        case Decision::Double: return ITable::Action::Double;
      break;
    default:
      return ITable::Action::Stand;
      break;
    }
  }
    break;
  case kPB_DrunkPlayer:
    return static_cast<ITable::Action>((rand()%4));
    break;
  case kPB_FearOfSuccess:
    return ITable::Action::Stand;
    break;
  case kPB_DegenerateGambler:
    return ITable::Action::Hit;
    break;
  default:
    break;
  }
}

int MGMPlayer::DecideInitialBet(const ITable& table, int player_index){
  int current_money = table.GetPlayerMoney(player_index);
  int bet = 0;
  switch (behaivour_)
  {
  case kPB_MatCorrect:
    bet = current_money / 4;
    break;
  case kPB_DrunkPlayer:
    bet = ((rand()%current_money + 1));
    break;
  case kPB_FearOfSuccess:
    bet = 100;
    break;
  case kPB_DegenerateGambler:
    bet = current_money / 2;
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

    break;
  }
}

/*
switch (behaivour_)
  {
  case kPB_MatCorrect:

    break;
  case kPB_DrunkPlayer:

    break;
  case kPB_FearOfSuccess:

    break;
  case kPB_DegenerateGambler:

    break;
  default:

    break;
  }
*/