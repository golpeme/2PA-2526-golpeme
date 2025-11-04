#include "MGMparcheesi.h"
#include "game.h"
#include "MGMPlayer.h"
#include <Windows.h>


int main(){
  SetConsoleOutputCP(CP_UTF8);
  SetConsoleCP(CP_UTF8);
  MGMParcheesi myp;
  MGMPlayer p0(0);
  MGMPlayer p1(1);
  MGMPlayer p2(2);
  MGMPlayer p3(3);
  const IPlayer* players[4]={&p0, &p1, &p2, &p3};   //
  Game g{myp,players};

  while(g.IsGameOver() == IParcheesi::Color::None){
    g.PlayTurn();
  }

  return 0;
}

