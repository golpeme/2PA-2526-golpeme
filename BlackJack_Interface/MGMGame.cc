#pragma once
#include "MGMGame.h"
#include "MGMTable.h"
#include "MGMRules.h"
#include "MGMPlayer.h"

void MGMGame::PrintPlayerBehaivour(int player_index, MGMPlayer::PlayerBehaivour behaivour) {
	switch (behaivour)
	{
	case MGMPlayer::kPB_MatCorrect:
		printf("\nPlayer %d entered the table, he is known for being mathematically good in the casino\n", player_index);
		break;
	case MGMPlayer::kPB_DrunkPlayer:
		printf("\nPlayer %d entered the table, he might had had one to many drinks\n", player_index);

		break;
	case MGMPlayer::kPB_FearOfSuccess:
		printf("\nPlayer %d entered the table, he really doesn't want to be here\n", player_index);

		break;
	case MGMPlayer::kPB_DegenerateGambler:
		printf("\nPlayer %d entered the table, if he could, he would bet his whole life savings on this table\n", player_index);

		break;
	default:
		break;
	}
}

void MGMGame::PrintPlayerAction(const ITable& table, const ITable::Action player_action, int player_index, int hand_index) {
	switch (player_action)
	{
	case ITable::Action::Double: {
		printf("\nPlayer %d has doubled\n", player_index);

		ITable::Hand hand = table.GetHand(player_index, hand_index);
		if (hand.size() > 0) {
			printf("\nNew Card\n");
			DrawCard(hand.back());
		}
		break;
	}
	case ITable::Action::Hit: {
		printf("\nPlayer %d has hit\n", player_index);

		ITable::Hand hand = table.GetHand(player_index, hand_index);
		if (hand.size() > 0) {
			printf("\nNew Card\n");
			DrawCard(hand.back());
		}

		break;
	}
	case ITable::Action::Split:
		printf("\nPlayer %d has split\n", player_index);
		break;
	case ITable::Action::Stand:
		printf("\nPlayer %d has stand\n", player_index);
		break;
	default:
		printf("\n?\n");
		break;
	}
}

void MGMGame::DrawRoundEndInfo(const std::vector <ITable::RoundEndInfo::BetResult>& result, int player_index) {
	for (int i = 0; i < result.size(); i++)
	{
		ITable::RoundEndInfo::BetResult res = result[i];
		if (res == ITable::RoundEndInfo::BetResult::Win) {
			printf("Player %d hand %d won\n", player_index, i);
		}
		else if (res == ITable::RoundEndInfo::BetResult::Lose) {
			printf("Player %d hand %d lost\n", player_index, i);
		}
		else if (res == ITable::RoundEndInfo::BetResult::Tie) {
			printf("Player %d hand %d tied\n", player_index, i);
		}
	}
}

void MGMGame::DrawCard(const ITable::Card& card) {
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
		DrawCard(card);
	printf("\n");
}

void MGMGame::PlayGame() {
	std::random_device seed;
	std::mt19937 rand(seed());

	players.resize(num_players);

	BaseRules rules;

	//int set_num_players = 0;
	//printf("set player num:");
	//scanf_s("%d", &set_num_players);

	MGMTable table(num_players, rules);

	for (int i = 0; i < num_players; i++)
	{
		players[i].SetRandomBehaivour();
		table.SetPlayer(i, &players[i]);
		PrintPlayerBehaivour(i, players[i].GetPlayerBehaivour(players[i]));
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
	do
	{
		printf("\n------------------------------\n--------round starting--------\n------------------------------\n");

		int round_players = current_players();

		table.CleanTable();
		table.StartRound();

		printf("\n------------------------------\n--------Deck Shuffled and filled--------\n------------------------------\n");
		printf("\n------------------------------\n--------Make your bets!--------\n------------------------------\n");



		// 1) initial bets
		for (int i = 0; i < round_players; i++)
		{
			//if (table.GetPlayerInitialBet(i) > 0) continue;
			//printf("\n\n\n******************\n\n\n*round players %d*\n\n\n******************\n\n\n", round_players);
			int bet = table.GetPlayer(i)->DecideInitialBet(table, i);
			table.PlayInitialBet(i, bet);
			printf("\n Player #%d bets %d $\n", i, bet);
		}
		printf("\nbets done\n");
		// 2) initial deal
		for (int i = 0; i < round_players; i++)
		{
			for (int j = 0; j < rules.InitialCards(); j++) {
				table.DealCard(i, 0);
			}
		}

		printf("\nDealer card\n");
		DrawCard(table.GetDealerCard());

		for (int i = 0; i < round_players; i++)
		{
			int hand_num = table.GetNumberOfHands(i);

			for (int j = 0; j < hand_num; j++) {
				ITable::Hand hand_to_draw = table.GetHand(i, j);
				printf("\nHand %d of player %d\n", j, i);
				for (auto& card : hand_to_draw)
				{
					DrawCard(card);
				}
			}
		}
		// 3) Safe bet / insurance
		for (int i = 0; i < round_players; i++)
		{
			players[i].DecideUseSafe(table, i);
		}
		// 4) player turns
		for (int i = 0; i < round_players; i++)
		{
			for (int j = 0; j < table.GetNumberOfHands(i); j++)
			{
				ITable::Action current_player_action = players[i].DecidePlayerAction(table, i, j);
				table.ApplyPlayerAction(i, j, current_player_action);
				PrintPlayerAction(table, current_player_action, i, j);
			}
		}
		// 5) Close round

		table.DealerRevealSecondCard();
		printf("\nDealer hand\n");
		int iter = 0;
		for (const ITable::Card& card : table.GetDealerHand())
		{
			DrawCard(card);
			iter++;
		}
		printf("\n\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n\n %d \n\n++++++++++++++++++++++++++++++++++++++", iter);
		ITable::RoundEndInfo print_info = table.FinishRound();
		for (int i = 0; i < round_players; i++)
		{
			DrawRoundEndInfo(print_info.winners[i], i);
			printf("\nplayer %d money -> %d$\n", i, table.GetPlayerMoney(i));
		}
		printf("\nDealer money -> %d$\n", table.DealerMoney());
	} while (any_alive_players() && table.DealerMoney() > 0);
}