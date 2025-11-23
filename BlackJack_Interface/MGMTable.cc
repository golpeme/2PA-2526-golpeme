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

int MGMTable::ValueToInteger(Hand hand){
    switch (hand.value)
    {
    case constant expression:
        /* code */
        break;
    
    default:
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

MGMTable::Result MGMTable::ApplyPlayerAction(int player_index, int hand_index, Action action) {

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