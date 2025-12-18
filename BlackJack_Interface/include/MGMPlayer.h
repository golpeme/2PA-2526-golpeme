#pragma once
#include "IPlayer.h"
#include "MGMTable.h"
#include "cstdlib"
#include "ctime"

/// @class MGMPlayer
/// @brief Blackjack player with configurable behavioral strategies.
/// 
/// Implements four behavior types: mathematically correct (basic strategy),
/// drunk (random), fearful (conservative), and degenerate (aggressive).
/// Each behavior determines decision-making throughout the game.
class MGMPlayer : public IPlayer {
  public: 

    /// @enum PlayerBehaivour
    /// @brief AI behavioral strategies for player decision-making.
    enum PlayerBehaivour {
      kPB_MatCorrect = 0,      ///< Plays using strategy matrix
      kPB_DrunkPlayer,         ///< Makes random decisions
      kPB_FearOfSuccess,       ///< Conservative, stands and bets low
      kPB_DegenerateGambler,   ///< Aggressive, prefers hits or doubles
      kPB_End                  ///< Sentinel value for enum
    };

    /// @brief Constructs a player with specific game rules.
    /// @param rules Reference to the game rules in effect
    MGMPlayer(BaseRules& rules);
    ~MGMPlayer() = default;

    ITable::Action DecidePlayerAction(const ITable& table, int player_index, int hand_index) override;
    int DecideInitialBet(const ITable& table, int player_index) override;
    bool DecideUseSafe(const ITable& table, int player_index) override;

    /// @brief Randomly assigns one of four behavioral strategies.
    /// Uses uniform distribution to select kPB_MatCorrect through kPB_DegenerateGambler.
    void SetRandomBehaivour();

    /// @brief Retrieves this player's current behavior strategy.
    /// @param player Reference to the player object
    /// @return Current PlayerBehaivour enum value
    PlayerBehaivour GetPlayerBehaivour(MGMPlayer player) { return player.behaivour_; };

  private:
    BaseRules& rules_;                            ///< Reference to game rules
    PlayerBehaivour behaivour_ = kPB_MatCorrect;  ///< Current behavioral strategy
    int player_index_;                            ///< Player's table position
    int hand_index_;                              ///< Current hand being played
    ITable::Hand player_hand_;                    ///< Cards in player's hand

    /// @enum Decision
    /// @brief Possible blackjack actions.
    enum class Decision : int {
      Stand = 0,   ///< Keep current hand total
      Hit,         ///< Request another card
      Split,       ///< Divide pair into two hands
      Double,      ///< Double bet and take one card
      end = 4      ///< Sentinel value
    };

// visible dealer card:
//      2                 3                 4               5                 6        
//      7                 8                 9               10                A    

/// @brief Basic strategy matrix for mathematically optimal play.
/// Rows represent hand totals/types, columns represent dealer's visible card.
/// Indexed by: hard totals (<=8 to 16+), soft totals (A-2 to A-7), pairs.
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


    /// @brief Converts a card to its blackjack numeric value.
    /// Aces=11, Face cards=10, Number cards=face value.
    /// @param card Card to evaluate
    /// @return Numeric value of the card
    int GetCardValue(ITable::Card card);

    /// @struct HandInfo
    /// @brief Encapsulates analysis information about a hand.
    struct HandInfo{
      bool is_pair;   ///< True if hand is a matching pair
      bool is_soft;   ///< True if hand contains Ace counted as 11
      int total;      ///< Total hand value
    };

    /// @brief Analyzes a hand and returns its characteristics.
    /// Computes whether hand is a pair, soft, and total value.
    /// @param hand Vector of cards to analyze
    /// @return HandInfo structure with analysis results
    HandInfo HandData(const ITable::Hand& hand);
    /// @brief Determines optimal move using the basic strategy matrix.
    /// @param info HandInfo describing current hand type and value
    /// @param dealer_card Dealer's visible upcard
    /// @return Optimal Decision (Hit, Stand, Split, Double)
    Decision GetMatCorrectDecision(HandInfo info, ITable::Card dealer_card);

    /// @brief Calculates row index in strategy matrix for hard totals.
    /// Maps hand totals (<=8 to 16+) to matrix row indices.
    /// @param total The hard hand total value
    /// @return Row index in mat_correct_behaivour_ matrix
    int HardRowIdx(int total);

    /// @brief Calculates row index in strategy matrix for soft totals.
    /// Maps soft totals (A-2 to A-7) to matrix row indices.
    /// @param total The soft hand total value
    /// @return Row index in mat_correct_behaivour_ matrix
    int SoftRowIdx(int total);

    /// @brief Calculates row index in strategy matrix for pairs.
    /// Maps pair ranks (2-10, A) to matrix row indices.
    /// @param pair_rank The rank of cards in the pair
    /// @return Row index in mat_correct_behaivour_ matrix
    static int PairRowIdx(int pair_rank);
};

