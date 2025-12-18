#pragma once
#include "IGame.h"
#include "MGMPlayer.h"
#include "MGMTable.h"
#include <random>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <iostream>
#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>

using namespace ftxui;

/// @class MGMGame
/// @brief Main game controller managing the blackjack game loop.
/// 
/// Orchestrates the complete game including initialization, round management,
/// player interactions, and game termination. Integrates GameRenderer for UI display.
class MGMGame : public IGame {
public:
  /// @brief Constructs a game instance with a table and players.
  /// @param table Reference to the game table
  /// @param players Vector of pointers to player objects
  MGMGame(MGMTable& table, std::vector<MGMPlayer*> players);

  void PlayGame() override;

private:
  MGMTable& table_;                    ///< Reference to the game table
  std::vector<MGMPlayer*> players_;    ///< Vector of game players

  /// @brief Checks if any player still has money remaining.
  /// @return true if at least one player has funds greater than 0
  bool AnyAlivePlayers();

  /// @brief Counts how many players can participate in the current round.
  /// @param rules Reference to current game rules
  /// @return Number of players with sufficient funds to place minimum bet
  int CountCurrentPlayers(const BaseRules& rules);
};


/// @class GameRenderer
/// @brief Terminal UI renderer for the blackjack game using FTXUI.
/// 
/// Handles all visual elements including cards, hands, game states,
/// player information, and user prompts. Provides consistent styling and colors.
class GameRenderer {
public:

  /// @brief Renders the main gameplay screen.
  /// Displays dealer hand, player hand, current bet, and money status.
  /// @param table Reference to the game table
  /// @param player_index Index of the player to display
  /// @param hand_index Index of the hand to display (for splits)
  /// @return FTXUI Element containing the complete game screen
  Element DrawGameScreen(MGMTable& table, int player_index, int hand_index);

  /// @brief Renders a single playing card with border and suit color.
  /// @param card The card to render
  /// @return FTXUI Element representing the card
  Element DrawCard(const ITable::Card& card);

  /// @brief Renders a complete hand of cards horizontally arranged.
  /// @param hand Vector of cards to display
  /// @return FTXUI Element with all cards in the hand
  Element DrawHand(const ITable::Hand& hand);

  /// @brief Renders the dealer's visible hand during play.
  /// @param hand The dealer's hand
  /// @return FTXUI Element with "DEALER" label and hand
  Element DrawDealerHand(const MGMTable::Hand& hand);

  /// @brief Renders the dealer's final hand at round conclusion.
  /// @param hand The dealer's final hand
  /// @return FTXUI Element with "FINAL DEALER HAND" label
  Element DrawFinalDealerHand(const MGMTable::Hand& hand);

  /// @brief Renders a money amount with color coding (green/red).
  /// @param money Amount to display
  /// @return FTXUI Element with formatted money value
  Element DrawBalance(int money);

  /// @brief Renders the player behavior introduction screen.
  /// @param player_index Index of the player
  /// @param behaviour The player's behavioral strategy
  /// @return FTXUI Element describing the player
  Element DrawPlayerBehaviourScreen(int player_index, MGMPlayer::PlayerBehaivour behaviour);

  /// @brief Renders the result of a player's action.
  /// @param action The action taken (Hit, Stand, Double, Split)
  /// @param player_index Index of the player
  /// @return FTXUI Element showing the action with color coding
  Element DrawActionResultScreen(const ITable::Action action, int player_index);

  /// @brief Renders individual round result for a single player.
  /// @param info Round end information for all players
  /// @param player_index Index of the player
  /// @param player_money Final money amount for this player
  /// @return FTXUI Element showing Win/Lose/Tie and money
  Element DrawRoundEndScreen(const ITable::RoundEndInfo& info, int player_index, int player_money);

  /// @brief Renders the betting screen for a player.
  /// @param player_index Index of the player
  /// @param bet Amount of the bet
  /// @return FTXUI Element showing bet amount
  Element DrawBettingScreen(int player_index, int bet);

  /// @brief Renders summary of all players' results for the round.
  /// Shows each player's outcome and final money plus dealer balance.
  /// @param info Round end information containing all results
  /// @param player_money Vector with final money for each player
  /// @param dealer_money Dealer's final money amount
  /// @return FTXUI Element with round summary table
  Element DrawRoundSummaryScreen(const ITable::RoundEndInfo& info, const std::vector<int>& player_money, int dealer_money);

  /// @brief Renders the game end screen with final standings.
  /// @param final_money Vector with final money for each player
  /// @param dealer_money Dealer's final money amount
  /// @return FTXUI Element with game end summary
  Element DrawGameEndScreen(const std::vector<int>& final_money, int dealer_money);

  /// @brief Renders a pause screen prompting for user input.
  /// @return FTXUI Element with pause message in a box
  Element DrawPauseScreen();

private:
  /// @brief Converts card value to single character string.
  /// Maps ACE→"A", TWO→"2", ..., KING→"K"
  /// @param value Card value enum
  /// @return String representation of the card value
  std::string ValueToString(ITable::Value value);

  /// @brief Converts card suit to single character string.
  /// Maps HEARTS→"H", DIAMONDS→"D", CLUBS→"C", SPADES→"S"
  /// @param suit Card suit enum
  /// @return String representation of the card suit
  std::string SuitToString(ITable::Suit suit);

  /// @brief Gets the color for a given suit.
  /// HEARTS→Red, DIAMONDS→Orange, CLUBS→Blue, SPADES→Black
  /// @param suit Card suit enum
  /// @return FTXUI Color for the suit
  Color GetSuitColor(ITable::Suit suit);
};
