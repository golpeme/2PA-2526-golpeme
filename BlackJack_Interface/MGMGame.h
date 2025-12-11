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
    const int num_players = 4;
    std::vector<MGMPlayer> players;

    void DrawCard(const ITable::Card& card);
    void PrintPlayerAction(const ITable& table, const ITable::Action player_action, int player_index, int hand_index);
    void MGMGame::DrawRoundEndInfo(const std::vector <ITable::RoundEndInfo::BetResult>& result, int player_index);
    void PrintPlayerBehaivour(int player_index, MGMPlayer::PlayerBehaivour behaivour);
};

/*
class GameRenderer {
  public: 
    void DrawHand(const ITable::Hand& hand);
    void DrawDealerHand(const ITable::Hand& hand);
    void DrawBalance(int money);
};*/