#pragma once
#ifndef ESAT_BLACKJACK_INTERFACES_IPLAYER_H
#define ESAT_BLACKJACK_INTERFACES_IPLAYER_H

#include "itable.h"

/**
 * @interface IPlayer
 * @brief Interface for player decision-making strategies in Blackjack.
 *
 * This interface defines the contract for implementing different player behaviors
 * in a Blackjack game. Implementations can represent human players, AI players,
 * or different strategy patterns.
 */
class IPlayer {
    public:
        virtual ~IPlayer() = default;

        /**
         * @brief Decides the action to take for a specific hand.
         *
         * @param table Reference to the game table containing current game state
         * @param player_index Index of the player making the decision
         * @param hand_index Index of the hand to decide action for (supports split hands)
         * @return ITable::Action The action chosen (e.g., hit, stand, double down, split)
         */
        virtual ITable::Action DecidePlayerAction(const ITable& table, int player_index, int hand_index) = 0;

        /**
         * @brief Decides the initial bet amount for a round.
         *
         * @param table Reference to the game table containing current game state
         * @param player_index Index of the player making the bet
         * @return int The bet amount to place
         */
        virtual int DecideInitialBet(const ITable& table, int player_index) = 0;

        /**
         * @brief Decides whether to use a safe/insurance option when the dealer gets an ace.
         *
         * @param table Reference to the game table containing current game state
         * @param player_index Index of the player making the decision
         * @return bool True if the player wants to use the safe option, false otherwise
         */
        virtual bool DecideUseSafe(const ITable& table, int player_index) = 0;
};

#endif // ESAT_BLACKJACK_INTERFACES_IPLAYER_H