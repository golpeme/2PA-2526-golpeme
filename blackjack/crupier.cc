#include "deck.cc"
#include "crupier.h"


void CardListInit(CardList* list){
         list->head = nullptr;
         list->tail = nullptr;
  list->size = 0;
}

void CardListFree(CardList* list){
  Node* aux = list->head;
  while(aux){
    Node* next = aux->next;
    free(aux);
    aux = next;
  }

  list->head = nullptr;
  list->tail = nullptr;
  list->size = 0;
}

int CardListSize(const CardList* list){ return list->size; }

Node* CreateCardNode(Card *card){
  Node* new_node = (Node*) malloc(sizeof(Node));
  if(!new_node){
    printf("error allocating card node");
    return nullptr;
  }
  new_node->data = *card;
  new_node->next = nullptr;
  new_node->prev = nullptr;

  return new_node;
}

Node* AddCardToList(CardList* list, Card card){
  Node* new_node = CreateCardNode(&card);
  if(!new_node) return nullptr;

  new_node->prev = list->tail;
  if(list->tail){
    list->tail->next = new_node;
  }else{
    list->head = new_node;
  }

  list->tail = new_node;
  list->size += 1;
  return new_node;
}

Crupier::Crupier() : next_deck_idx(0){
  ResetAndShuffle();
}

void Crupier::ResetAndShuffle(){
  current_deck = Deck();
  ShuffleDeck(current_deck.deck);
  next_deck_idx = 0;
}

Card Crupier::DrawCard(){
  if (next_deck_idx >= Deck::kCardTotal)
  {
    ShuffleDeck(current_deck.deck);
    next_deck_idx = 0;
  }
  drawn_card = current_deck.deck[next_deck_idx++];
  return drawn_card;
}

Card Crupier::DealCards(){
  return DrawCard();
}

CrupierAction Crupier::DecideAction(int current_value) const{
  return (current_value < stand_value) ? kCA_SelfDeal : kCA_Stand;
}
