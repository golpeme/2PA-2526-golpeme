#pragma once
#ifndef ESAT_BLACKJACK_INTERFACES_IGAMES_H
#define ESAT_BLACKJACK_INTERFACES_IGAMES_H

/**
 * @interface IGame
 * @brief Interface for game execution and control.
 *
 * This interface defines the contract for implementing game logic.
 * Any class implementing this interface should provide the complete
 * game flow and execution logic.
 */
class IGame {
    public:
        virtual ~IGame() = default;    
        /**
         * @brief Executes the main game loop.
         *
         * This method contains the complete game flow, including initialization,
         * game rounds, and cleanup. It should handle the entire lifecycle of
         * a game session from start to finish.
         */
        virtual void PlayGame() = 0;
};

#endif // ESAT_BLACKJACK_INTERFACES_IGAMES_H