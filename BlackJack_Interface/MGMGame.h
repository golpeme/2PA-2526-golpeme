#include "IGame.h"
#include "MGMPlayer.h"
#include "MGMTable.h"

class MGMGame: public IGame {
  public:
      MGMGame() = default;

    void PlayGame() override;
};

void PlayGame() {
    MGMTable table();
    MGMTable::table.StartRound();
    
}