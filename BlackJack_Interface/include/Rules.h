#pragma once
#ifndef ESAT_BLACKJACK_INTERFACES_RULES_H
#define ESAT_BLACKJACK_INTERFACES_RULES_H

/**
 * @class BaseRules
 * @brief Base class defining the core rules and configuration for Blackjack games.
 *
 * This class provides default game rules and settings that can be customized
 * through inheritance. It defines the fundamental parameters that control
 * game behavior.
 *
 * @section game_end_conditions Game End Conditions
 * - A player loses when they run out of money
 * - The dealer loses when they run out of money
 * - The game ends when either the dealer loses or all players lose
 *
 * @section player_ranking Player Ranking
 * Players are ranked in the following order:
 * - First: Players still at the table at the end of the game, ranked by money amount
 * - Second: Players who have lost, in reverse order of elimination (last eliminated first)
 */
class BaseRules {
    public:
        virtual ~BaseRules() = default;

        /**
         * @brief Gets the winning point threshold.
         * @return int The score needed to win (default: 21)
         */
        virtual int GetWinPoint() const { return 21; }

        /**
         * @brief Gets the number of card decks used in the game.
         * @return int Number of decks (default: 1)
         */
        virtual int NumberOfDecks() const { return 1; }

        /**
         * @brief Gets the number of cards dealt to each player at the start.
         * @return int Number of initial cards (default: 2)
         */
        virtual int InitialCards() const { return 2; }

        /**
         * @brief Gets the initial amount of money for each player.
         * @return int The starting money amount for players
         */
        virtual int InitialPlayerMoney() const { return 4000; }

        /**
         * @brief Gets the initial amount of money for the dealer.
         * @return int The starting money amount for the dealer
         */
        virtual int InitialDealerMoney() const { return 100000; }

        /**
         * @brief Gets the minimum initial bet.
         * @return int The minimum initial bet amount
         */
        virtual int MinimumInitialBet() const { return 100; }

        /**
         * @brief Gets the maximum initial bet.
         * @return int The maximum initial bet amount
         */
        virtual int MaximumInitialBet() const { return 10000; }

        /**
         * @brief Gets the dealer's stop threshold.
         * @return int The score at which the dealer must stop drawing cards (default: 17)
         */
        virtual int DealerStop() const { return 17; }
};

#endif // ESAT_BLACKJACK_INTERFACES_RULES_H