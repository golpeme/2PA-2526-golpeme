#pragma once
#ifndef ESAT_BLACKJACK_INTERFACES_ITABLE_H
#define ESAT_BLACKJACK_INTERFACES_ITABLE_H

#include <vector>

/**
 * @interface ITable
 * @brief Interface representing the Blackjack game table and its operations.
 *
 * This interface provides access to game state information and methods to manipulate
 * the game flow. It handles player hands, bets, card dealing, and round management.
 *
 * @note The dealer is part of the table and has no choices beyond applying the rules.
 */
class ITable {
	public:
		virtual ~ITable() = default;

		/**
		 * @enum Action
		 * @brief Available player actions during a Blackjack round.
		 */
		enum class Action {
				Stand,  ///< Player stands with current hand
				Hit,    ///< Player requests another card
				Double, ///< Player doubles bet and receives one more card
				Split   ///< Player splits a pair into two separate hands
		};

		/**
		 * @enum Result
		 * @brief Result of an operation on the table.
		 */
		enum class Result {
				Ok,      ///< Operation completed successfully
				Illegal  ///< Operation was illegal/invalid
		};

		/**
		 * @enum Value
		 * @brief Card values in the deck.
		 */
		enum class Value {
				ACE = 1, ///< Ace (can be 1 or 11)
				TWO,     ///< Two
				THREE,   ///< Three
				FOUR,    ///< Four
				FIVE,    ///< Five
				SIX,     ///< Six
				SEVEN,   ///< Seven
				EIGHT,   ///< Eight
				NINE,    ///< Nine
				TEN,     ///< Ten
				JACK,    ///< Jack (worth 10)
				QUEEN,   ///< Queen (worth 10)
				KING,    ///< King (worth 10)
				end      ///< Sentinel value
		};

		/**
		 * @enum Suit
		 * @brief Card suits in the deck.
		 */
		enum class Suit {
				HEARTS,   ///< Hearts suit
				CLUBS,    ///< Clubs suit
				SPADES,   ///< Spades suit
				DIAMONDS, ///< Diamonds suit
				end       ///< Sentinel value
		};

		/**
		 * @struct Card
		 * @brief Represents a playing card with value and suit.
		 */
		struct Card {
				Value value_; ///< The card's value
				Suit suit_;   ///< The card's suit
		};

		/// Type alias for a hand of cards
		using Hand = std::vector<Card>;

		/**
		 * @struct RoundEndInfo
		 * @brief Information about the end of a round.
		 */
		struct RoundEndInfo {
			Hand dealer_hand;                   ///< The dealer's final hand

			enum class BetResult {
				Win,
				Lose,
				Tie,
				end
			};
				/// Whether each hand of each player won, lost, or tied
				/// Indexed as [player_index][hand_index]
				std::vector<std::vector<BetResult>> winners;
				std::vector<int> player_money_delta;  ///< Money won by each player in this round (money is already deducted when the bet is placed)
				int dealer_money_delta;             ///< Money won or lost by the dealer in this round
		};

		/// Maximum number of players at the table (reduce if running out of cards during a round)
		static const int kMaxPlayers = 7;

		/**
		 * @brief Gets a specific hand for a player.
		 *
		 * @param player_index Index of the player
		 * @param hand_index Index of the hand
		 * @return Hand The requested hand
		 */
		virtual Hand GetHand(int player_index,int hand_index) const = 0;

		/**
		 * @brief Gets the current number of hands for a player.
		 *
		 * @param player_index Index of the player
		 * @return int Number of hands the player currently has
		 */
		virtual int GetNumberOfHands(int player_index) const = 0;

		/**
		 * @brief Gets the current total bet for a specific hand.
		 *
		 * @param player_index Index of the player
		 * @param hand_index Index of the hand
		 * @return int The current bet amount for this hand
		 */
		virtual int GetPlayerCurrentBet(int player_index,int hand_index) const = 0;

		/**
		 * @brief Gets the remaining unbet money for a player.
		 *
		 * @param player_index Index of the player
		 * @return int The player's remaining money not currently bet
		 */
		virtual int GetPlayerMoney(int player_index) const = 0;

		/**
		 * @brief Gets the initial bet amount placed by a player.
		 *
		 * @param player_index Index of the player
		 * @return int The money the player bet in their initial bet
		 */
		virtual int GetPlayerInitialBet(int player_index) const = 0;

		/**
		 * @brief Gets the dealer's visible card.
		 *
		 * @return Card The dealer's face-up card
		 */
		virtual Card GetDealerCard() const = 0;

		/**
		 * @brief Gets the dealer's current money.
		 *
		 * @return int The dealer's money amount
		 */
		virtual int DealerMoney() const = 0;

		/**
		 * @brief Deals a card to a specific player hand.
		 *
		 * The dealer is part of the table and has no choices beyond applying the rules.
		 *
		 * @param player_index Index of the player receiving the card
		 * @param hand_index Index of the hand receiving the card
		 */
		virtual void DealCard(int player_index,int hand_index) = 0;

		/**
		 * @brief Places an initial bet for a player.
		 *
		 * @param player_index Index of the player placing the bet
		 * @param money Amount of money to bet
		 * @return Result Ok if the bet was valid, Illegal otherwise
		 */
		virtual Result PlayInitialBet(int player_index,int money) = 0;

		/**
		 * @brief Places a safe/insurance bet for a player.
		 *
		 * @param player_index Index of the player placing the safe bet
		 * @return Result Ok if the bet was valid, Illegal otherwise
		 */
		virtual Result PlaySafeBet(int player_index) = 0;

		/**
		 * @brief Applies a player action to a specific hand.
		 *
		 * @param player_index Index of the player taking the action
		 * @param hand_index Index of the hand to apply the action to
		 * @param action The action to perform (Stand, Hit, Double, Split)
		 * @return Result Ok if the action was valid, Illegal otherwise
		 */
		virtual Result ApplyPlayerAction(int player_index,int hand_index,Action action) = 0;

		/**
		 * @brief Prepares the start of a round.
		 *
		 * This method performs the following operations:
		 * - Shuffles the deck
		 * - Deals one card to the dealer
		 */
		virtual void StartRound() = 0;

		/**
		 * @brief Calculates the end of a round.
		 *
		 * This method performs the following operations:
		 * - Plays the dealer's hand according to rules
		 * - Determines whether each player wins or loses
		 * - Resolves bet winnings
		 *
		 * @return RoundEndInfo Information about the round results
		 */
		virtual RoundEndInfo FinishRound() = 0;

		/**
		 * @brief Cleans up the table after a round.
		 * 
		 * This method performs the following operations:
		 * - Clears hands, clears bets, and returns cards to the deck if necessary
		 */
		virtual void CleanTable() = 0;
};

#endif // ESAT_BLACKJACK_INTERFACES_ITABLE_H
