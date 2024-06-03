# Sudoku Game Implementation

This is a C-based console application for playing Sudoku, a popular number puzzle game. The implementation allows users to play Sudoku in a terminal, offering features like undo/redo moves, saving/loading games, and a timer for gameplay.

## Features

- **Dynamic Sudoku Board Generation:** Generates a solvable Sudoku board each time the game starts.
- **Undo/Redo:** Allows the player to undo or redo moves, enhancing the gameplay experience.
- **Save/Load Game:** Players can save their current game state to a file and load it later to continue from where they left off.
- **Timer:** An optional timer can be enabled to limit the duration of the game, adding an extra challenge.
- **Input Validation:** Checks for duplicate numbers in rows, columns, and 3x3 boxes to ensure valid moves.

## Getting Started

### Prerequisites

- A C compiler (like GCC or Clang)
- Standard C libraries

### Compilation

To compile the game, use the following command in your terminal:

```bash
gcc Sudoku.c -o Sudoku
```

Replace `sudoku_game.c` with the path to the source file if necessary.

### Running the Game

After compilation, you can start the game using:

```bash
./Sudoku
```

Follow the on-screen instructions to play the game.

## Usage

1. **Start the Game:** The game starts with a partially filled Sudoku board. The objective is to fill the empty cells with numbers from 1 to 9.
2. **Make a Move:** Enter the row number, column number, and the value you wish to place in the specified cell.
3. **Undo/Redo Moves:** Choose the undo or redo option if you wish to revert or reapply your last action.
4. **Save/Load Game:** You can save your game progress at any time and load it later to continue.
5. **Exit:** You can exit the game anytime by selecting the exit option.

## How It Works

- The game initializes with a menu offering options to start a new game, load a game, enable/disable the timer, and exit.
- The Sudoku board is populated with a valid solution and then partially emptied based on the difficulty level.
- Players input their moves, and the game validates these moves against Sudoku rules.
- The undo/redo stack keeps track of the moves for the undo and redo functionality.
- The game checks for a win condition (all cells correctly filled) or end of the timer to conclude the gameplay.
