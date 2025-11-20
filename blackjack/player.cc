#include "player.h"

void Hand::Init(int initial_bet){
  CardListInit(&cards);
  bet = initial_bet;
  finished = false;
}

void Hand::Clear(){
  CardListFree(&cards);
  bet = 0;
  finished = true;
}

void Hand::AddCard(Card card){
  AddCardToList(&cards, card);
}

int Hand::Value() const{
  int total = 0;
  int aces = 0;
  for(Node* n = cards.head; n; n = n->next){
    total += n->data.card_value;
    if(n->data.card_type == kCard_Ace) aces++; 
  }
  while(total > 21 && aces > 0){
    total -= 10;
    aces--;
  }
  return total;
}

bool Hand::IsBlackjack() const {
  if (Value() == 21)
  {
    return true;
  }else{
    return false;
  }
}

bool Hand::IsBust() const {
  if (Value() > 21)
  {
    return true;
  }else{
    return false;
  }
}

bool Hand::CanSplit() const{
  if(cards.size != 2) return false;
  Card a = cards.head->data;
  Card b = cards.head->next->data;

  int value_a = (a.card_type == kCard_Ace) ? 11 : a.card_value;
  int value_b = (b.card_type == kCard_Ace) ? 11 : b.card_value;
  if(value_a == 10 && value_b == 10) return true;

  if (value_a == value_b)
  {
    return true;
  }else{
    return false;
  }
}

void Player::Init(PlayerBehaivour bhv, int money_init, int base_bet_init){
  behaivour = bhv;
  money = money_init;
  base_bet = base_bet_init;
  hand_count = 0;
  for (int i = 0; i < 4; i++)
  {
    hands[i].Clear();
  }
}

void Player::NewRound(){
  for (int i = 0; i < 4; i++)
  {
    hands[i].Clear();
  }
  hand_count = 1;
  hands[0].Init(base_bet);
}

void Player::RecieveCard(int hand_idx, Card card){
  if(hand_idx < 0 || hand_idx >= hand_count) return;
  hands[hand_idx].AddCard(card);
}

void Player::FinishHand(int hand_idx){
  if(hand_idx < 0 || hand_idx >= hand_count) return;
  hands[hand_idx].finished = true;
}

bool Player::CanDouble(const Hand& hand) const{
  int value = hand.Value();
  if ((hand.cards.size == 2) && (value >= 9 && value <= 11))
  {
    return true;
  }else{
    return false;
  }
}

PlayerAction Player::DecideAction(const Hand& hand, Card dealer_card) const{
  if(hand.CanSplit()){
    if (behaivour == kPB_MatCorrect || behaivour == kPB_DegenerateGambler)
    {
      return kPA_Split;
    }
  }
  int value = hand.Value();
  switch (behaivour)
  {
  case kPB_MatCorrect:
    if(hand.cards.size == 2 && (value == 9 || value == 10 || value == 11)) return kPA_Double;
    if(value <= 11) return kPA_Hit;
    if(value >= 17) return kPA_Stand;
    return kPA_Hit;
    break;
  case kPB_DrunkPlayer:
    if((rand()% 100) < 70){
      return kPA_Hit;
    }else{
      return kPA_Stand;
    }
    break;
  case kPB_FearOfSuccess:
    if(value > 10) return kPA_Stand;
    else return kPA_Hit;
    break;
  case kPB_DegenerateGambler:
    if(hand.cards.size == 2) return kPA_Double;
    else return kPA_Hit;
    break;
  default:
    return kPA_Hit;
    break;
  }
}

bool Player::TrySplit(int hand_idx){
  if(hand_idx < 0 || hand_idx >= hand_count) return false;

  Hand& hand = hands[hand_idx];
  if(!hand.CanSplit() || hand_count >= 4) return false;

  Hand& new_hand = hands[hand_count];

  new_hand.Init(hand.bet);

  Node* second = hand.cards.head ? hand.cards.head->next : NULL;

  if(!second) return false;

  Card second_card = second->data;

  if(second == hand.cards.tail) hand.cards.tail = hand.cards.head;
  if(second && second->prev) second->prev->next = second->next;
  if(second && second->next) second->next->prev = second->prev;
  free(second);
  hand.cards.size -= 1;

  new_hand.AddCard(second_card);
  hand_count += 1;

  return true;
}