#pragma once
#ifndef DECK_H
#define DECK_H

#include <stdlib.h>
#include <stdio.h>

enum CardSuit{
  kSuit_Clubs = 0,
  kSuit_Diamonds,
  kSuit_Hearts,
  kSuit_Spades,
  kSuit_End
};

enum CardType{
  kFaceCard_Jack = 0,
  kFaceCard_Queen,
  kFaceCard_King,
  kNumbered_Card,
  kCard_Ace,
  kFaceCard_End
};

struct Card{
  CardType card_type; //4 bytes
  CardSuit card_suit; //4 bytes
  int card_value;     //4 bytes
};                    //12 bytes
                      //allocating memory for the max posible cards drawn without busting
                      //would cost 12 * 11 bytes per player + 12 * 11 bytes for the dealer cards
                      //resulting in a total of 588 bytes, for now cards can be stored in an array
                      //as memory won't be a problem

struct Deck{
  static const int kCardPerSuit = 13;
  static const int kSuitCount = 4;
  static const int kCardTotal = kCardPerSuit * kSuitCount;
  
  int deck[kCardTotal];
  Card drawn_card;

  Deck();
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
}

Node* AddCardToList(CardList* list, Card card){
  Node* new_node = CreateCardNode(&card);
  new_node->prev = list->tail;
  
  list
}

#endif