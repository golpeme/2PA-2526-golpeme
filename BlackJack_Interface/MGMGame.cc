#pragma once
#include "MGMGame.h"
#include "MGMTable.h"
#include "MGMRules.h"
#include "MGMPlayer.h"



void MGMGame::PlayGame() {
	std::random_device seed;
	std::mt19937 rand(seed());

	GameRenderer renderer;

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
		auto behaviour_screen = renderer.DrawPlayerBehaviourScreen(i, players[i].GetPlayerBehaivour(players[i]));
		auto screen = Screen::Create(Dimension::Full());
		Render(screen, behaviour_screen);
		screen.Print();
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

		int round_players = current_players();

		table.CleanTable();
		table.StartRound();

		Elements round_content;

		// 1) initial bets
		for (int i = 0; i < round_players; i++)
		{
			//if (table.GetPlayerInitialBet(i) > 0) continue;
			//printf("\n\n\n******************\n\n\n*round players %d*\n\n\n******************\n\n\n", round_players);
			/*int bet = table.GetPlayer(i)->DecideInitialBet(table, i);
			table.PlayInitialBet(i, bet);
			printf("\n Player #%d bets %d $\n", i, bet);*/

			int bet = table.GetPlayer(i)->DecideInitialBet(table, i);
			table.PlayInitialBet(i, bet);

			auto betting_screen = renderer.DrawBettingScreen(i, bet);
			auto screen = Screen::Create(Dimension::Full());
			Render(screen, betting_screen);
			screen.Print();
		}
		printf("\nbets done\n");
		// 2) initial deal
		for (int i = 0; i < round_players; i++)
		{
			for (int j = 0; j < rules.InitialCards(); j++) {
				table.DealCard(i, 0);
			}
		}

		auto dealer_element = renderer.DrawCard(table.GetDealerCard());
		auto screen = Screen::Create(Dimension::Full());
		Render(screen, dealer_element);

		/*for (int i = 0; i < round_players; i++)
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
		}*/

		for (int i = 0; i < round_players; i++) {
			int hand_num = table.GetNumberOfHands(i); {
				for (int j = 0; j < hand_num; j++)
				{
					auto screen_element = renderer.DrawGameScreen(table, i, j);
					auto screen = Screen::Create(Dimension::Full());
					Render(screen, screen_element);
					screen.Print();
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
				//PrintPlayerAction(table, current_player_action, i, j);

				auto action_screen = renderer.DrawActionResultScreen(current_player_action, i);
				auto screen = Screen::Create(Dimension::Full());
				Render(screen, action_screen);
				screen.Print();
			}
		}
		// 5) Close round

		table.DealerRevealSecondCard();

		ITable::RoundEndInfo print_info = table.FinishRound();

		for (int i = 0; i < round_players; i++)
		{
			/*DrawRoundEndInfo(print_info.winners[i], i);
			printf("\nplayer %d money -> %d$\n", i, table.GetPlayerMoney(i));*/
			auto end_screen = renderer.DrawRoundEndScreen(print_info, i, table.GetPlayerMoney(i));
			auto screen = Screen::Create(Dimension::Full());
			Render(screen, end_screen);
			screen.Print();
		}
	} while (any_alive_players() && table.DealerMoney() > 0);
}

//TUI IMPLEMENTATION

std::string GameRenderer::ValueToString(ITable::Value value) {
	switch (value) {
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
}

std::string GameRenderer::SuitToString(ITable::Suit suit) {
	switch (suit) {
	case ITable::Suit::HEARTS:   return "H";
	case ITable::Suit::DIAMONDS: return "D";
	case ITable::Suit::CLUBS:    return "C";
	case ITable::Suit::SPADES:   return "S";
	default: return "?";
	}
}

Color GameRenderer::GetSuitColor(ITable::Suit suit) {
	switch (suit)
	{
	case ITable::Suit::HEARTS: return Color::Red;
	case ITable::Suit::CLUBS: return Color::Blue;
	case ITable::Suit::SPADES: return Color::Black;
	case ITable::Suit::DIAMONDS: return Color::Orange1;
	default: return Color::Black;
	}
}

Element GameRenderer::DrawCard(const ITable::Card& card) {
	std::string value = ValueToString(card.value_);
	std::string suit = SuitToString(card.suit_);
	Color color = GetSuitColor(card.suit_);
	
	auto top = hbox({
		text(value) | ftxui::color(color) | bold,
		flex(filler()),
		});

	auto middle = hbox({
		filler(),
		text(suit) | ftxui::color(color) | bold | center,
		filler(),
		});

	auto bottom = hbox({
		flex(filler()),
		text(value) | ftxui::color(color) | bold,
		});

	return border(
		vbox({
			top,
			filler(),
			middle,
			filler(),
			bottom,
			}) | size(WIDTH, EQUAL, 10) | size(HEIGHT, EQUAL, 7)
			);
}
Element GameRenderer::DrawHand(const ITable::Hand& hand) {
	Elements cards;
	for (const auto& card : hand)
	{
		cards.push_back(DrawCard(card));
	}

	if (cards.empty()) return text("No cards left") | dim;

	return hbox(cards) | center;
}

Element GameRenderer::DrawDealerHand(const MGMTable::Hand& hand) {
	return vbox({
			text("DEALER") | bold | center,
			text(""),
			DrawHand(hand),
		});
}

Element GameRenderer::DrawBalance(int money) {
	std::string balance = "$" + std::to_string(money);

	Color color = (money > 0) ? Color::Green : Color::Red;

	return hbox({
			text(balance) | ftxui::color(color) | bold | center,
		});
}

Element GameRenderer::DrawGameScreen(MGMTable& table, int player_index, int hand_index) {
	auto player_hand = table.GetHand(player_index, hand_index);
	int player_money = table.GetPlayerMoney(player_index);
	int player_bet = table.GetPlayerCurrentBet(player_index, hand_index);

	return vbox({
		text("═══════════════════════════════════") | bold | center,
		text("BLACKJACK") | bold | center | color(Color::Yellow),
		text("═══════════════════════════════════") | bold | center,
		DrawDealerHand(table.GetDealerHand()),
		separator(),
		text("Player " + std::to_string(player_index)) | bold,
		DrawHand(player_hand),
		hbox({
				text("Bet: $" + std::to_string(player_bet)) | color(Color::Cyan),
				text(" | "),
				DrawBalance(player_money),
		}) | center,
		text(""),
		});

}

Element GameRenderer::DrawPlayerBehaviourScreen(int player_index, MGMPlayer::PlayerBehaivour behaviour) {
	std::string beh_text;

	switch (behaviour)
	{
	case MGMPlayer::kPB_MatCorrect:
		beh_text = " Entered the table, he is known for being mathematically good in the casino";
		break;
	case MGMPlayer::kPB_DrunkPlayer:
		beh_text = " Entered the table, he might had had one to many drinks";

		break;
	case MGMPlayer::kPB_FearOfSuccess:
		beh_text = " Entered the table, he really doesn't want to be here";

		break;
	case MGMPlayer::kPB_DegenerateGambler:
		beh_text = " Entered the table, if he could, he would bet his whole life savings on this table";
		break;
	default:
		beh_text = "unknowm";
		break;
	}

	return vbox({ text(""),
				text("Player " + std::to_string(player_index)) | bold | center | color(Color::Yellow),
				text(beh_text) | center | italic,
		});

}

Element GameRenderer::DrawActionResultScreen(const ITable::Action action, int player_index) {
	std::string action_text;
	Color action_color;

	switch (action) {
	case ITable::Action::Hit:
		action_text = "HIT";
		action_color = Color::Yellow;
		break;
	case ITable::Action::Stand:
		action_text = "STAND";
		action_color = Color::Green;
		break;
	case ITable::Action::Double:
		action_text = "DOUBLE";
		action_color = Color::Magenta;
		break;
	case ITable::Action::Split:
		action_text = "SPLIT";
		action_color = Color::Cyan;
		break;
	default:
		action_text = "UNKNOWN";
		action_color = Color::Red;
	}

	return vbox({
			text("Player " + std::to_string(player_index)) | bold,
			text("Action: " + action_text) | bold | center | color(action_color),
		});
}

Element GameRenderer::DrawRoundEndScreen(const ITable::RoundEndInfo& info, int player_index, int player_money){
	std::string result_text;
	Color result_color;

	if (player_index < info.winners.size() && info.winners[player_index].size() > 0) {
		auto res = info.winners[player_index][0];

		if (res == ITable::RoundEndInfo::BetResult::Win) {
			result_text = "YOU WIN!";
			result_color = Color::Green;
		}
		else if (res == ITable::RoundEndInfo::BetResult::Lose) {
			result_text = "YOU LOSE";
			result_color = Color::Red;
		}
		else {
			result_text = "TIE";
			result_color = Color::Yellow;
		}
	}

	return vbox({
			text(result_text) | bold | center | color(result_color),
			hbox({
					text("Player " + std::to_string(player_index) + " Money: "),
					text("$" + std::to_string(player_money)) | color(Color::Cyan) | bold,
			}) | center,
		});
}

Element GameRenderer::DrawBettingScreen(int player_index, int bet){
	return vbox({
			text("Make Your Bets!") | bold | center | color(Color::Yellow),
			text("Player " + std::to_string(player_index)) | bold | center,
			hbox({
					text("Bet: "),
					text("$" + std::to_string(bet)) | color(Color::Green) | bold,
			}) | center,
		});
}