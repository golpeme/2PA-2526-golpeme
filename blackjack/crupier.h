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


void CardListInit(CardList* list);
void CardListFree(CardList* list);
int CardListSize(const CardList* list);
Node* CreateCardNode(Card *card);
Node* AddCardToList(CardList* list, Card card);

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
       
       
