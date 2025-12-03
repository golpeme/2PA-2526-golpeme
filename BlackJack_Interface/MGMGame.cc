#pragma once
#include "MGMGame.h"
#include "MGMTable.h"
#include "MGMRules.h"
#include "MGMPlayer.h"

void MGMGame::DrawHand(const ITable::Hand& hand) {
	//lambda to cast value to string
	auto ValueToString = [](ITable::Value val) -> const char* {
		switch (val) {
			case ITable::Value::ACE:   return "A";
			case ITable::Value::TWO:   return "2";
			case ITable::Value::THREE: return "3";
			case ITable::Value::FOUR:  return "4";
			case ITable::Value::FIVE:  return "5";
			case ITable::Value::SIX:   return "6";
			case ITable::Value::SEVEN: return "7";
			case ITable::Value::EIGHT: return "8";
			case ITable::Value::NINE:  return "9";
			case ITable::Value::TEN:   return "10";
			case ITable::Value::JACK:  return "J";
			case ITable::Value::QUEEN: return "Q";
			case ITable::Value::KING:  return "K";
			default: return "?";
		}
	};

	//lambda to cast suits to icons
	auto SuitToString = [](ITable::Suit suit) -> const char* {
		switch (suit) {
			case ITable::Suit::HEARTS:   return "H";
			case ITable::Suit::DIAMONDS: return "D";
			case ITable::Suit::CLUBS:    return "C";
			case ITable::Suit::SPADES:   return "S";
			default: return "?";
		}
	};
	
	// Colores ANSI
	const char* RED = "\033[91m";      // Rojo para ♥ ♦
	const char* BLACK = "\033[30m";    // Negro para ♠ ♣
	const char* RESET = "\033[0m";     // Reset color
	const char* BOLD = "\033[1m";      // Negrita

	auto DrawCard = [ValueToString, SuitToString, RED, BLACK, RESET, BOLD](const ITable::Card& card) {
		const char* val = ValueToString(card.value_);
		const char* suit = SuitToString(card.suit_);

		// Determinar color según el palo
		const char* color = (card.suit_ == ITable::Suit::HEARTS || card.suit_ == ITable::Suit::DIAMONDS) ? RED : BLACK;

		printf("+-------+\n");
		printf("|%s%s%s       %s|\n", BOLD, color, val, RESET);
		printf("|       |\n");
		printf("|   %s%s%s   %s|\n", BOLD, color, suit, RESET);
		printf("|       |\n");
		printf("|     %s%s%s%s|\n", BOLD, color, val, RESET);
		printf("+-------+\n");
		};

	printf("\n");
	for (auto& card : hand) {

		DrawCard(card);
	}
	printf("\n");
}

void MGMGame::PlayGame() {
	std::random_device seed;
	std::mt19937 rand(seed());

	MGMRules rules;

	//int set_num_players = 0;
	//printf("set player num:");
	//scanf_s("%d", &set_num_players);

	const int num_players = 4;

	MGMTable table(num_players, rules);

	std::vector<MGMPlayer> players(num_players);
	for (int i = 0; i < num_players; i++)
	{
		players[i].SetRandomBehaivour();
		table.SetPlayer(i, &players[i]);
	}

	//saves a &in var
	//[&] captures ref said 
	// & can use vars like num_players and table (refs)
	//when money changes, lambda sees it updated
	//no params called as alive_players()
	auto any_alive_players = [&]() {
		for (int i = 0; i < table.GetPlayerNum(); i++)
		{
			if (table.GetPlayerMoney(i) > 0) return true;
		}
		return false;
	};

	auto current_players = [&]() {
		int current_player_num = 0;
		for (int i = 0; i < table.GetPlayerNum(); i++) {
			if (table.GetPlayerMoney(i) > rules.MinimumInitialBet())
			{
				current_player_num++;
			}
		}
		return current_player_num;
		};
		while (any_alive_players() && table.DealerMoney() > 0)
		{
			printf("\n------------------------------\n--------round starting--------\n------------------------------\n");
			
			int round_players = current_players();
			
			table.CleanTable();
			table.StartRound();
			
			printf("\n------------------------------\n--------Deck Shuffled and filled--------\n------------------------------\n");
			printf("\n------------------------------\n--------Make your bets!--------\n------------------------------\n");

			

			// 1) Apuestas iniciales
			for (int i = 0; i < round_players; i++)
			{
				//if (table.GetPlayerInitialBet(i) > 0) continue;
				//printf("\n\n\n******************\n\n\n*round players %d*\n\n\n******************\n\n\n", round_players);
				int bet = table.GetPlayer(i)->DecideInitialBet(table, i);
				table.PlayInitialBet(i, bet);
				printf("\n Player #%d bets %d $\n", i, bet);
			}
				printf("\nbets done\n");
			// 2) Reparto inicial a jugadores (mano 0)
			for (int i = 0; i < round_players; i++)
			{
				for (int j = 0; j < rules.InitialCards(); j++) {
					table.DealCard(i, 0);
				}
			}
			for (int i = 0; i < round_players; i++)
			{
				int hand_num = table.GetNumberOfHands(i);

				for (int j = 0; j < hand_num; j++){
					ITable::Hand hand_to_draw = table.GetHand(i, j);
					DrawHand(hand_to_draw);
				}

			}
			// 3) Safe bet / insurance si el dealer enseña As
			for (int i = 0; i < round_players; i++)
			{
				players[i].DecideUseSafe(table, i);
			}
			// 4) Turnos de jugadores (incluye splits)
			for (int i = 0; i < round_players; i++)
			{
				for (int j = 0; j < table.GetNumberOfHands(i); j++)
				{
					players[i].DecidePlayerAction(table, i, j);
				}
			}
			// 5) Cerrar ronda (dealer juega + pagar/aplicar resultados) y limpiar
			ITable::RoundEndInfo print_info = table.FinishRound();
			print_info.winners;
			printf("\nDealer money -> %d$\n", table.DealerMoney());
			for (int i = 0; i < round_players; i++)
			{
				printf("\nplayer %d money -> %d$\n", i, table.GetPlayerMoney(i));
			}
		}
}