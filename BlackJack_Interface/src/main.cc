#include "MGMGame.h"
#include "MGMRules.h"

#include <ftxui/screen/screen.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>

int main() {
  auto screen = ScreenInteractive::TerminalOutput();

  int selected_rules = 0;
  int selected_players = 1; // índice, no el número directamente


  std::vector<std::string> rules_options = { "Base Rules", "Disruptive Rules", "Fitfluencer Rules" };
  std::vector<std::string> players_options = { "2 Players", "3 Players", "4 Players", "5 Players", "6 Players" };
  std::vector<int> player_counts = { 2, 3, 4, 5, 6 };

  auto rules_menu = Radiobox(&rules_options, &selected_rules);
  auto players_menu = Radiobox(&players_options, &selected_players);

  auto button = Button("Start Game", [&] {
    screen.ExitLoopClosure()();
    });

  auto component = Container::Vertical({
      rules_menu,
      players_menu,
      button
    });

  auto renderer_comp = Renderer(component, [&] {
    return vbox({
        text("*******************************") | bold | center,
        text("BLACKJACK - SELECT MODE") | bold | center | color(Color::Yellow),
        text("*******************************") | bold | center,
        text(""),
        text("Rules:") | bold,
        rules_menu->Render() | border,
        text(""),
        text("Number of Players:") | bold,
        players_menu->Render() | border,
        text(""),
        button->Render() | center,
      }) | border;
    });

  screen.Loop(renderer_comp);

  // Después
  int num_players = player_counts[selected_players];
  // Crear las reglas
  BaseRules* rules = nullptr;
  if (selected_rules == 0) {
    rules = new BaseRules();
  }
  else if (selected_rules == 1) {
    rules = new DisruptiveRules();
  }
  else {
    rules = new FitfluencerRules();
  }

  // Crear tabla y jugadores
  MGMTable table(num_players, *rules);

  std::vector<MGMPlayer*> players;
  for (int i = 0; i < num_players; i++) {
    players.push_back(new MGMPlayer(*rules));
  }

  MGMGame game(table, players);
  game.PlayGame();

  delete rules;
  return 0;
}