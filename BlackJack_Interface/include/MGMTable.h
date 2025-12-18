#pragma once
#include "ITable.h"
#include "Rules.h"
#include "MGMPlayer.h"

/// @class MGMTable
/// @brief Manages the blackjack table state, deck, hands, and money flow.
/// 
/// Central game state manager handling deck creation/shuffling, card dealing,
/// hand tracking, betting, player elimination, and round completion logic.
/// Enforces dealer rules during hand evaluation.
class MGMTable: public ITable {
  public:
    /// @brief Constructs the table for a specific number of players.
    /// Initializes deck, player money, and dealer state.
    /// @param num_players Number of players (typically 1-6)  
    /// @param rules Reference to game rules governing play
    MGMTable(int num_players, const BaseRules& rules);

    Hand GetHand(int player_index,int hand_index) const override;
    int GetNumberOfHands(int player_index) const override;

    int GetPlayerMoney(int player_index) const override;
    
    int GetPlayerCurrentBet(int player_index,int hand_index) const override;
    int GetPlayerInitialBet(int player_index) const override;
    Result PlayInitialBet(int player_index,int money) override;

    
    Card GetDealerCard() const override;
    int DealerMoney() const override;
    
    void CleanTable() override;

    void DealCard(int player_index,int hand_index) override;

    Result PlaySafeBet(int player_index) override;
    Result ApplyPlayerAction(int player_index,int hand_index,Action action) override;

    void StartRound() override;

    RoundEndInfo FinishRound() override;

    /// @brief Fills the deck with all cards according to rules.
    /// Creates complete deck(s) based on NumberOfDecks rule.
    void FillDeck();

    /// @brief Randomly shuffles the current deck.
    void ShuffleDeck();


    /// @brief Registers a player at a specific table position.
    /// @param player_index Position for this player  
    /// @param player Pointer to the player object
    void SetPlayer(int player_index, IPlayer* player){
      players_[player_index] = player;
    }

    /// @brief Retrieves the player at a specific position.
    /// @param player_index Position of the player
    /// @return Pointer to the player object at this position
    IPlayer* GetPlayer(int player_index) const{
      return players_[player_index];
    }

    /// @brief Gets the total number of players at the table.
    /// @return Number of players (constant throughout game)
    int GetPlayerNum() const { return player_num_; };

    /// @brief Reveals the dealer's second (hole) card.
    /// Called at the end of player actions, before dealer plays.
    void DealerRevealSecondCard();
    /// @brief Retrieves the dealer's complete hand.
    /// @return Vector of cards in the dealer's hand
    Hand GetDealerHand();


  private:
    std::vector<std::vector<Hand> > hands_;              ///< hands_[player_index][hand_index]
    std::vector<std::vector<int> > player_bets_;         ///< player_bets_[player_index][hand_index]
    std::vector<int> total_player_money_;                ///< Money for each player
    std::vector<IPlayer*> players_;                      ///< Player pointers
    int player_num_;                                     ///< Total number of players
    std::vector<Card> deck_;                             ///< Current deck of cards
    Hand dealer_hand_;                                   ///< Dealer's cards
    int dealer_money_;                                   ///< Dealer's money
    const BaseRules& rules_;                             ///< Game rules reference
    const int kSuitNum = 4;                              ///< Number of suits
    const int kValueNum = 13;                            ///< Number of card values

    /// @brief Converts a card to its numeric blackjack value.
    /// Aces=11, Face cards=10, Number cards=face value.
    /// @param card The card to evaluate
    /// @return Numeric value of the card
    int GetCardValue(Card card);

    /// @struct HandInfo
    /// @brief Analysis information about a hand's composition.
    struct HandInfo {
      bool is_pair;   ///< Whether hand contains matching pair
      bool is_soft;   ///< Whether hand is soft (Ace counts as 11)
      int total;      ///< Total hand value
    };

    /// @brief Eliminates a player due to insufficient funds.
    /// @param player_index Index of the player to remove
    void RemovePlayers(int player_index);

    /// @brief Analyzes a hand and computes its characteristics.
    /// @param hand Vector of cards to analyze
    /// @return HandInfo with pair status, soft status, and total value
    HandInfo HandData(const Hand& hand);
};