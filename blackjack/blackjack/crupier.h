#include "deck.h"
#include <time.h>
#include <stdlib.h>


enum CrupierAction{
  kCA_Deal = 0,
  kCA_SelfDeal,
  kCA_Stand,
  kCA_End
};

struct Node{
  Card data;
  Node* prev;
  Node* next;
};

struct CardList{
  Node* head;
  Node* tail;
  int size;
};

void CardListInit(CardList* list);
void CardListFree(CardList* list);
int CardListSize(const CardList* list);
Node* CreateCardNode(Card *card);
Node* AddCardToList(CardList* list, Card card);

struct Crupier
{
  static const int stand_value = 16;

  Deck current_deck;
  int next_deck_idx;
  Card drawn_card;

  Crupier();
  void ResetAndShuffle();

  CardType CardTypeInfoOutput(Card drawn_card);
  Card DrawCard();
  Card DealCards();
  CrupierAction DecideAction(int current_value) const;
};

Crupier dealer;


/*Card DrawCard(){
  Card aux_card;

  aux_card.card_value = rand() % 13 + 1;
  aux_card.card_suit = static_cast<CardSuit> (rand() % 4);
  if (aux_card.card_value == 1 || aux_card.card_value == 11)
  {
    aux_card.card_type = kCard_Ace;
  }else if(aux_card.card_value == 10){
    aux_card.card_type = CardTypeInfoOutput();
  }else {
    aux_card.card_type = kNumbered_Card;
  }

  dealer.drawn_card = aux_card;

  return dealer.drawn_card;
}*/



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