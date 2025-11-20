#include "IPlayer.h"
#include "MGMRules.h"
#include "cstdlib"
#include "ctime"


class MGMPlayer : public IPlayer {
  public: 

    enum PlayerBehaivour{
      kPB_MatCorrect = 0,
      kPB_DrunkPlayer,
      kPB_FearOfSuccess,
      kPB_DegenerateGambler,
      kPB_End
    };

    MGMPlayer() = default;

    ITable::Action DecidePlayerAction(const ITable& table, int player_index, int hand_index) override;
    int DecideInitialBet(const ITable& table, int player_index) override;
    bool DecideUseSafe(const ITable& table, int player_index) override;
    void SetRandomBehaivour();

  private:
    PlayerBehaivour behaivour_ = kPB_MatCorrect;


    int player_index_;
    int hand_index_;
    ITable::Hand player_hand_;

};

void MGMPlayer::SetRandomBehaivour(){
  behaivour_ = static_cast<PlayerBehaivour>((rand()%4));
}

ITable::Action MGMPlayer::DecidePlayerAction(const ITable& table, int player_index, int hand_index){
  switch (behaivour_)
  {
  case kPB_MatCorrect:
    /* code */
    break;
  case kPB_DrunkPlayer:
    /* code */
    break;
  case kPB_FearOfSuccess:
    /* code */
    break;
  case kPB_DegenerateGambler:
    /* code */
    break;
  default:
    break;
  }
}

int MGMPlayer::DecideInitialBet(const ITable& table, int player_index){
  int current_money = table.GetPlayerMoney(player_index);
  int bet = 0;
  switch (behaivour_)
  {
  case kPB_MatCorrect:
    bet = current_money / 2;
    break;
  case kPB_DrunkPlayer:
    while(bet > table && bet < rules->MaximumInitialBet()){
      bet = ((rand()%current_money + 1));
    }
    break;
  case kPB_FearOfSuccess:
    bet = rules->MinimumInitialBet();
    break;
  case kPB_DegenerateGambler:
    bet = current_money - rules->MinimumInitialBet();
    break;
  default:
    bet = rules->MinimumInitialBet();
    break;
  }
  return bet;
}

bool MGMPlayer::DecideUseSafe(const ITable& table, int player_index){

}