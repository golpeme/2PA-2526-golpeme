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
    void DrawHand(const ITable::Hand& hand);


};

/*
class GameRenderer {
  public: 
    void DrawHand(const ITable::Hand& hand);
    void DrawDealerHand(const ITable::Hand& hand);
    void DrawBalance(int money);
};*/