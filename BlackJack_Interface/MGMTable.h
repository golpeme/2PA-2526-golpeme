#pragma once
#include "ITable.h"
#include "Rules.h"
#include "MGMPlayer.h"


class MGMTable: public ITable {
  public:
    MGMTable(int num_players, const BaseRules& rules);

    Hand GetHand(int player_index,int hand_index) const override;
    int GetNumberOfHands(int player_index) const override;

    int GetPlayerMoney(int player_index) const override;
    
    int GetPlayerCurrentBet(int player_index,int hand_index) const override;
    int GetPlayerInitialBet(int player_index) const override;
    Result PlayInitialBet(int player_index,int money) override;

    
    Card GetDealerCard() const override;
    int DealerMoney() const override;
    
    void CleanTable() override;

    // Dealer es parte de la mesa, no tiene elecciones propias mas alla de aplicar las reglas
    void DealCard(int player_index,int hand_index) override;

    Result PlaySafeBet(int player_index) override;
    Result ApplyPlayerAction(int player_index,int hand_index,Action action) override;

    void StartRound() override;

    RoundEndInfo FinishRound() override;
    void FillDeck();
    void ShuffleDeck();

    void SetPlayer(int player_index, IPlayer* player){
      players_[player_index] = player;
    }

    IPlayer* GetPlayer(int player_index) const{
      return players_[player_index];
    }


  private:
    std::vector<std::vector<Hand> > hands_;              //hands_[player_index][hand_index];
    std::vector<std::vector<int> > player_bets_;         //player_bets_[player_index][hand_index];
    std::vector<int> total_player_money_;                //total_player_money_[player_index];
    std::vector<IPlayer*> players_;
    int num_players_;
    std::vector<Card> deck_;
    Hand dealer_hand_;
    int dealer_money_;
    const BaseRules& rules_;
    const int kSuitNum = 4;
    const int kValueNum = 13;
    int GetCardValue(Card card);
    int player_num_ = 0;
    
    struct HandInfo{
      //can be split
      bool is_pair;
      //hard hand -> ace as 1
      //soft hand -> ace as 11 (can be worth 1)
      bool is_soft;
      int total;
    };
    
    HandInfo HandData(const Hand& hand);
};