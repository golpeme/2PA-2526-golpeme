#pragma once
#include "IGame.h"
#include "MGMPlayer.h"
#include "MGMTable.h"
#include <random>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>

using namespace ftxui;

class MGMGame : public IGame {
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


class GameRenderer {
public:

  Element DrawGameScreen(MGMTable& table, int player_index, int hand_index);

  Element DrawCard(const ITable::Card& card);
  Element DrawHand(const ITable::Hand& hand);
  Element DrawDealerHand(const MGMTable::Hand& hand);
  Element DrawBalance(int money);

  Element DrawPlayerBehaviourScreen(int player_index, MGMPlayer::PlayerBehaivour behaviour);
  Element DrawActionResultScreen(const ITable::Action action, int player_index);
  Element DrawRoundEndScreen(const ITable::RoundEndInfo& info, int player_index, int player_money);
  Element DrawBettingScreen(int player_index, int bet);

private:
  std::string ValueToString(ITable::Value value);
  std::string SuitToString(ITable::Suit suit);
  Color GetSuitColor(ITable::Suit suit);
};