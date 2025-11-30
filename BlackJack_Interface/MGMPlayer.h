#pragma once
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

    enum class Decision : int{
      Stand = 0,
      Hit,
      Split,
      Double,
      end = 4,
    };

// visible dealer card:
//      2                 3                 4               5                 6        
//      7                 8                 9               10                A    
std::vector<std::vector<Decision> > mat_correct_behaivour_ = {
        // Hard totals
        {Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit,
        Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit},  // <= 8

        {Decision::Hit,   Decision::Double,Decision::Double,Decision::Double,Decision::Double,
        Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit},  // 9

        {Decision::Double,Decision::Double,Decision::Double,Decision::Double,Decision::Double,
        Decision::Double,Decision::Double,Decision::Double,Decision::Hit,   Decision::Hit},  // 10

        {Decision::Double,Decision::Double,Decision::Double,Decision::Double,Decision::Double,
        Decision::Double,Decision::Double,Decision::Double,Decision::Double,Decision::Hit},  // 11

        {Decision::Hit,   Decision::Hit,   Decision::Stand, Decision::Stand, Decision::Stand,
        Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit},  // 12

        {Decision::Stand, Decision::Stand, Decision::Stand, Decision::Stand, Decision::Stand,
        Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit},  // 13

        {Decision::Stand, Decision::Stand, Decision::Stand, Decision::Stand, Decision::Stand,
        Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit},  // 14

        {Decision::Stand, Decision::Stand, Decision::Stand, Decision::Stand, Decision::Stand,
        Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit},  // 15

        {Decision::Stand, Decision::Stand, Decision::Stand, Decision::Stand, Decision::Stand,
        Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit},  // 16

        {Decision::Stand, Decision::Stand, Decision::Stand, Decision::Stand, Decision::Stand,
        Decision::Stand, Decision::Stand, Decision::Stand, Decision::Stand, Decision::Stand}, // >= 17

        // Soft totals (A + X)
        {Decision::Stand, Decision::Double,Decision::Double,Decision::Double,Decision::Double,
        Decision::Stand, Decision::Stand, Decision::Hit,   Decision::Hit,   Decision::Hit},  // A - 7

        {Decision::Hit,   Decision::Double,Decision::Double,Decision::Double,Decision::Double,
        Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit},  // A - 6

        {Decision::Hit,   Decision::Hit,   Decision::Double,Decision::Double,Decision::Double,
        Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit},  // A - 5

        {Decision::Hit,   Decision::Hit,   Decision::Double,Decision::Double,Decision::Double,
        Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit},  // A - 4

        {Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Double,Decision::Double,
        Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit},  // A - 3

        {Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Double,Decision::Double,
        Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit},  // A - 2

        // Pairs
        {Decision::Split, Decision::Split, Decision::Split, Decision::Split, Decision::Split,
        Decision::Split, Decision::Split, Decision::Split, Decision::Split, Decision::Split}, // A - A

        {Decision::Split, Decision::Split, Decision::Split, Decision::Split, Decision::Split,
        Decision::Stand, Decision::Split, Decision::Split, Decision::Stand, Decision::Stand}, // 9 - 9

        {Decision::Split, Decision::Split, Decision::Split, Decision::Split, Decision::Split,
        Decision::Split, Decision::Split, Decision::Split, Decision::Split, Decision::Split}, // 8 - 8

        {Decision::Split, Decision::Split, Decision::Split, Decision::Split, Decision::Split,
        Decision::Split, Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit},  // 7 - 7

        {Decision::Split, Decision::Split, Decision::Split, Decision::Split, Decision::Split,
        Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit},  // 6 - 6

        {Decision::Double,Decision::Double,Decision::Double,Decision::Double,Decision::Double,
        Decision::Double,Decision::Double,Decision::Double,Decision::Hit,   Decision::Hit},  // 5 - 5

        {Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit,
        Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit},  // 4 - 4

        {Decision::Hit,   Decision::Hit,   Decision::Split, Decision::Split, Decision::Split,
        Decision::Split, Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit},  // 3 - 3

        {Decision::Hit,   Decision::Hit,   Decision::Split, Decision::Split, Decision::Split,
        Decision::Split, Decision::Hit,   Decision::Hit,   Decision::Hit,   Decision::Hit},  // 2 - 2

        {Decision::Stand, Decision::Stand, Decision::Stand, Decision::Stand, Decision::Stand,
        Decision::Stand, Decision::Stand, Decision::Stand, Decision::Stand, Decision::Stand}, // 10 - 10
    };

    int GetCardValue(ITable::Card card);

    struct HandInfo{
      //can be split
      bool is_pair;
      //hard hand -> ace as 1
      //soft hand -> ace as 11 (can be worth 1)
      bool is_soft;
      int total;
    };

    HandInfo HandData(const ITable::Hand& hand);
    Decision GetMatCorrectDecision(HandInfo info, ITable::Card dealer_card);

    //table helpers
    int HardRowIdx(int total);
    int SoftRowIdx(int total);
    static int PairRowIdx(int pair_rank);
};

