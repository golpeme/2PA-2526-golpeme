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
};