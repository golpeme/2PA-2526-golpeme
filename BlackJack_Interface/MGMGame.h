#pragma once
#include "IGame.h"
#include "MGMPlayer.h"
#include "MGMTable.h"
#include <random>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>

class MGMGame: public IGame {
  public:
    MGMGame() = default;
    void PlayGame() override;
  private:
    void DrawCard(const ITable::Card& card);
    void PrintPlayerAction(const ITable::Action player_action, int player_index);
    void DrawRoundEndInfo(const ITable::RoundEndInfo::BetResult& res, int player_index);


};

/*
class GameRenderer {
  public: 
    void DrawHand(const ITable::Hand& hand);
    void DrawDealerHand(const ITable::Hand& hand);
    void DrawBalance(int money);
};*/