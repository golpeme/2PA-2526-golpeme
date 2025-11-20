#include <vector>
#include "Rules.h"
#include "MGMTable.h"
#include "MGMPlayer.h"
#include "MGMGame.h"

int main() {
    srand((unsigned)time(0));

    BaseRules rules;

    MGMTable t{};
    MGMPlayer p1{MGMPlayer::GetRandomBehaivour},p2{},p3{},p4{},p5{},p6{},p7{};
    MGMGame game{};

    game.PlayGame();








    do {// RONDA
        // APUESTA INICIAL
        // REPARTIR CARTAS A JUGADORES
        // REPARTIR CARTA AL CROUPIER
        IF croupier as
            POR CADA JUGADOR
                PREGUNTAR SEGURO
        POR CADA JUGADOR
            EL JUGADOR HACE ACCIONES HASTA QUE PARA O SE PASA
                SI SE PARTE LA MANO, JUEGAS UN TURNO COMPLETO POR CADA MANO
        JUEGA EL CROUPIER
        SE VALORA VICTORIA/DERROTA
        SE REPARTE EL DINERO
        SE LIMPIA LA MESA
    } while (COMPROBAR CONDICIONES DE VICTORIA/DERROTA);
}


        // CheckPlayersLeft
        // CleanHands
        // CleanBets
        // PlaceInitialBet
        // Shuffle
        // Deal 2 cards per player
        // Deal 1 card to dealer

    // InsuranceOffer
        // Player is asked if they want insurance, in case of an ace

    // Fpr each player they can do actions until they overflow or stand
    // PlayerTurn
        // Player is given the board to check and
        // reply with an action
        // Stand
        // Hit
        // Double
        // Split

    // When all players have stood, dealer draws out cards up to their max



    ShuffleCards
    PlaceBet
    MinBet
    MaxBet

    DealerStop

    PlayerCount

    
    