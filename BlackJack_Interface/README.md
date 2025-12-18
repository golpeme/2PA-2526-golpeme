# MGM Blackjack

A terminal-based Blackjack game implemented in C++ with AI players and multiple rule variants. Features a colorful TUI powered by [FTXUI](https://github.com/ArthurSonzogni/FTXUI).

## Features

- **Multiple Rule Sets**: Base Rules (standard 21), Disruptive Rules (target 25), and Fitfluencer Rules (target 20)
- **AI Player Behaviors**: 
  - Mathematically Correct (basic strategy)
  - Drunk Player (random decisions)
  - Fear of Success (always stands)
  - Degenerate Gambler (always hits)
- **Full Game Actions**: Hit, Stand, Double Down, Split
- **2-6 Players** support
- **Colorful Terminal UI** with card rendering

## Prerequisites

- **Windows 10/11**
- **Visual Studio 2022** (with C++ desktop development workload)
- **Premake5** ([Download here](https://premake.github.io/download))
- **Git**

## Installation

### 1. Clone the repository with submodules


git clone --recursive https://github.com/golpeme/2PA-2526-golpeme.git
cd MGM-Blackjack

If you already cloned without `--recursive`, initialize the submodules:

git submodule update --init --recursive

### 2. Build FTXUI library

Navigate to the FTXUI submodule and build it:

cd ftxui
mkdir build && cd build
cmake .. -DCMAKE_INSTALL_PREFIX=../install
cmake --build . --config Release
cmake --install . --config Release
cd

### 3. Generate Visual Studio solution

Make sure `premake5` is in your PATH, then run:

premake5 vs2022

This generates the solution files in the `build/` directory.

### 4. Compile the project

**Option A: Using the batch file**

compile.bat

**Option B: Using Visual Studio**
1. Open `build/BlackJack.sln` in Visual Studio 2022
2. Set configuration to `Debug` and platform to `x64`
3. Build Solution (`Ctrl+Shift+B`)

**Option C: Using MSBuild directly**

cd build
"C:\Program Files\Microsoft Visual Studio\2022\Community\MSBuild\Current\Bin\MSBuild.exe" BlackJack.sln /p:Configuration=Debug /p:Platform=x64


## Running the Game

After compilation, the executable is located at:


build/bin/Debug/BlackJack.exe


Run it from the terminal:

cd build/bin/Debug
BlackJack.exe

Or simply double-click the executable.

## How to Play

1. **Select Rules**: Choose between Base, Disruptive, or Fitfluencer rules
2. **Select Players**: Choose 2-6 AI players
3. **Watch the Game**: Each player is assigned a random behavior
4. **Press any key** to advance through rounds
5. **Game ends** when all players lose or the dealer goes bankrupt

### Rule Variants

| Rule Set    | Win Point | Decks | Initial Cards | Dealer Stops  |
|-------------|-----------|-------|---------------|---------------|
| Base        | 21        | 1     | 2             | 17            |
| Disruptive  | 25        | 2     | 3             | 21            |
| Fitfluencer | 20        | 1     | 2             | 16            |

## Project Structure

MGM-Blackjack/
├── include/
│   ├── IGame.h          # Game interface
│   ├── IPlayer.h        # Player interface
│   ├── ITable.h         # Table interface
│   ├── Rules.h          # Base rules class
│   ├── MGMGame.h        # Game implementation + Renderer
│   ├── MGMPlayer.h      # Player implementation + AI behaviors
│   ├── MGMTable.h       # Table implementation
│   └── MGMRules.h       # Rule variants
├── src/
│   ├── main.cc          # Entry point
│   ├── MGMGame.cc       # Game logic + TUI rendering
│   ├── MGMPlayer.cc     # Player decisions
│   ├── MGMTable.cc      # Table state management
│   └── MGMRules.cc      # Rules implementation
├── ftxui/               # FTXUI submodule
├── build/               # Generated build files
├── premake5.lua         # Build configuration
├── compile.bat          # Build script
└── README.md
```

## Troubleshooting

### "FTXUI libraries not found"
Make sure you built and installed FTXUI to `ftxui/install/`. Check that these files exist:
- `ftxui/install/lib/ftxui-screen.lib`
- `ftxui/install/lib/ftxui-dom.lib`
- `ftxui/install/lib/ftxui-component.lib`

### "Premake5 not recognized"
Add Premake5 to your system PATH or place `premake5.exe` in the project root.

### "MSBuild not found"
Use the Visual Studio Developer Command Prompt, or update the path in `compile.bat` to match your VS installation.