#include "mock_parcheesi.h"
#include "game.h"

int main(){
  MockParcheesi myp;
  const IPlayer* players[4]={nullptr,nullptr,nullptr,nullptr};
  Game g{myp,players};

  while(g.IsGameOver() == IParcheesi::Color::None){
    g.PlayTurn();
  }

  return 0;
}

