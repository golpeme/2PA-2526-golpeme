#include "blackjack.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>

int main() {
  srand(time(NULL));
  BlackJack table(1);
  table.StartRound();
  return 0;
}