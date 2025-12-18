#pragma once
#include "MGMGame.h"
#include "MGMTable.h"
#include "MGMRules.h"
#include "MGMPlayer.h"


MGMGame::MGMGame(MGMTable& table, std::vector<MGMPlayer*> players)
	: table_(table), players_(players) {
}

void MGMGame::PlayGame() {
	std::random_device seed;
	std::mt19937 rand(seed());

	GameRenderer renderer;

	BaseRules rules;


	for (int i = 0; i < players_.size(); i++)
	{
		players_[i]->SetRandomBehaivour();
		table_.SetPlayer(i, players_[i]);
		auto behaviour_screen = renderer.DrawPlayerBehaviourScreen(i, players_[i]->GetPlayerBehaivour(*players_[i]));
		auto screen = Screen::Create(Dimension::Fit(behaviour_screen));
		Render(screen, behaviour_screen);
		screen.Print();
		printf("\n");
	}

	do
	{

		int round_players = CountCurrentPlayers(rules);

		table_.CleanTable();
		table_.StartRound();

		Elements round_content;

		// 1) initial bets
		for (int i = 0; i < round_players; i++)
		{

			int bet = table_.GetPlayer(i)->DecideInitialBet(table_, i);
			table_.PlayInitialBet(i, bet);

			auto betting_screen = renderer.DrawBettingScreen(i, bet);
			auto screen = Screen::Create(Dimension::Fit(betting_screen));
			Render(screen, betting_screen);
			screen.Print();
			printf("\n");
		}

		// 2) initial deal
		for (int i = 0; i < round_players; i++)
		{
			for (int j = 0; j < rules.InitialCards(); j++) {
				table_.DealCard(i, 0);
			}
		}

		auto dealer_element = renderer.DrawCard(table_.GetDealerCard());
		auto screen = Screen::Create(Dimension::Fit(dealer_element));
		Render(screen, dealer_element);

		for (int i = 0; i < round_players; i++) {
			int hand_num = table_.GetNumberOfHands(i); {
				for (int j = 0; j < hand_num; j++)
				{
					auto screen_element = renderer.DrawGameScreen(table_, i, j);
					auto screen = Screen::Create(Dimension::Fit(screen_element));
					Render(screen, screen_element);
					screen.Print();
					printf("\n");
				}
			}
		}

		// 3) Safe bet / insurance
		for (int i = 0; i < round_players; i++)
		{
			players_[i]->DecideUseSafe(table_, i);
		}
		// 4) player turns
		for (int i = 0; i < round_players; i++)
		{
			for (int j = 0; j < table_.GetNumberOfHands(i); j++)
			{
				ITable::Action current_player_action = players_[i]->DecidePlayerAction(table_, i, j);
				table_.ApplyPlayerAction(i, j, current_player_action);
				//PrintPlayerAction(table, current_player_action, i, j);

				auto action_screen = renderer.DrawActionResultScreen(current_player_action, i);
				auto screen = Screen::Create(Dimension::Fit(action_screen));
				Render(screen, action_screen);
				screen.Print();
				printf("\n");
			}
		}
		// 5) Close round

		table_.DealerRevealSecondCard();

		ITable::RoundEndInfo print_info = table_.FinishRound();

		std::vector<int> money_list;
		for (int i = 0; i < round_players; i++) {
			money_list.push_back(table_.GetPlayerMoney(i));
		}

		auto final_dealer_hand = renderer.DrawFinalDealerHand(table_.GetDealerHand());
		auto screen_final_dealer_hand = Screen::Create(Dimension::Fit(final_dealer_hand));
		Render(screen_final_dealer_hand, final_dealer_hand);
		screen_final_dealer_hand.Print();
		printf("\n");

		for (int i = 0; i < round_players; i++)
		{
			auto end_screen = renderer.DrawRoundEndScreen(print_info, i, table_.GetPlayerMoney(i));
			auto screen = Screen::Create(Dimension::Fit(end_screen));
			Render(screen, end_screen);
			screen.Print();
			printf("\n");
		}

		auto summary_screen = renderer.DrawRoundSummaryScreen(print_info, money_list, table_.DealerMoney());
		auto summary = Screen::Create(Dimension::Fit(summary_screen));
		Render(summary, summary_screen);
		summary.Print();
		std::cout << "\n";

		auto pause = renderer.DrawPauseScreen();
		auto pause_screen =	Screen::Create(Dimension::Fit(pause));
		Render(pause_screen, renderer.DrawPauseScreen());
		pause_screen.Print();
		std::cin.get();

	} while (AnyAlivePlayers() && table_.DealerMoney() > 0);

	std::vector<int> final_money;
	for (int i = 0; i < players_.size(); i++) {
		final_money.push_back(table_.GetPlayerMoney(i));
	}

	auto end_screen = renderer.DrawGameEndScreen(final_money, table_.DealerMoney());
	auto screen = Screen::Create(Dimension::Fit(end_screen));
	Render(screen, end_screen);
	screen.Print();

}

bool MGMGame::AnyAlivePlayers() {
	for (int i = 0; i < table_.GetPlayerNum(); i++) {
		if (table_.GetPlayerMoney(i) > 0) return true;
	}
	return false;
}

int MGMGame::CountCurrentPlayers(const BaseRules& rules) {
	int current_player_num = 0;
	for (int i = 0; i < table_.GetPlayerNum(); i++) {
		if (table_.GetPlayerMoney(i) > rules.MinimumInitialBet()) {
			current_player_num++;
		}
	}
	return current_player_num;
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
		filler(),
		});

	auto middle = hbox({
		filler(),
		text(suit) | ftxui::color(color) | bold,
		filler(),
		});

	auto bottom = hbox({
		filler(),
		text(value) | ftxui::color(color) | bold,
		});

	return border(
		vbox({
			top,
			filler(),
			middle,
			filler(),
			bottom,
			}) | size(WIDTH, EQUAL, 8) | size(HEIGHT, EQUAL, 5)
			);
}

Element GameRenderer::DrawHand(const ITable::Hand& hand) {
	Elements cards;
	for (const auto& card : hand)
	{
		cards.push_back(DrawCard(card));
		cards.push_back(text(" ")); // Espacio entre cartas
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

Element GameRenderer::DrawFinalDealerHand(const MGMTable::Hand& hand) {
	return vbox({
			text("FINAL DEALER HAND") | bold | center,
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
		text("***********************************") | bold | center,
		text("BLACKJACK") | bold | center | color(Color::Yellow),
		text("***********************************") | bold | center,
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

Element GameRenderer::DrawGameEndScreen(const std::vector<int>& final_money, int dealer_money) {
	Elements player_results;

	for (int i = 0; i < final_money.size(); i++) {

		player_results.push_back(hbox({
				text("Player " + std::to_string(i) + " went home crying with: ") | bold,
				text("$" + std::to_string(final_money[i])) | color(Color::Cyan)
			}) | center);
	}

	player_results.push_back(text(""));
	player_results.push_back(hbox({
			text("Dealer ended up getting: ") | bold,
			text("$" + std::to_string(dealer_money)) | color(Color::Orange1)
		}) | center);

	return vbox({






			text("") | center,
			text("*******************************") | bold | center,
			text("GAME OVER") | bold | center | color(Color::Red),
			text("*******************************") | bold | center,
			text(""),
			vbox(player_results) | border,
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

Element GameRenderer::DrawPauseScreen() {
	return vbox({
		text(""),
		text("Press any key to continue...") | bold | center | color(Color::Yellow),
		text(""),
		}) | border;
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

Element GameRenderer::DrawRoundSummaryScreen(const ITable::RoundEndInfo& info, const std::vector<int>& player_money, int dealer_money) {
	Elements results;

	for (int i = 0; i < info.winners.size(); i++) {
		std::string result_text = "LOSE";
		Color result_color = Color::Red;

		if (i < info.winners.size() && info.winners[i].size() > 0) {
			if (info.winners[i][0] == ITable::RoundEndInfo::BetResult::Win) {
				result_text = "WIN";
				result_color = Color::Green;
			}
			else if (info.winners[i][0] == ITable::RoundEndInfo::BetResult::Tie) {
				result_text = "TIE";
				result_color = Color::Yellow;
			}
		}

		results.push_back(hbox({
				text("Player " + std::to_string(i) + ": ") | bold,
				text(result_text) | color(result_color) | bold,
				text(" | $" + std::to_string(player_money[i])) | color(Color::Cyan)
			}));
	}

	results.push_back(text(""));
	results.push_back(hbox({
			text("Dealer: ") | bold,
			text("$" + std::to_string(dealer_money)) | color(Color::Orange1) | bold
		}));

	return vbox({
			text("") | center,
			text("*******************************") | bold | center,
			text("ROUND SUMMARY") | bold | center | color(Color::Yellow),
			text("*******************************") | bold | center,
			text(""),
			vbox(results) | border,
			text(""),
		});
}