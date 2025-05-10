# Sudoku Game in C

![sudoku](https://github.com/user-attachments/assets/8376326a-16b8-4999-8494-8bc5a8494d52)


## 🎮 Overview

This is a console-based Sudoku game implemented in C, featuring:
- Multiple board sizes (4x4, 9x9, 16x16)
- Three difficulty levels (Easy, Medium, Hard)
- Game saving/loading functionality
- Hints system
- Game statistics tracking
- Simulated Annealing solver 

## 🛠️ Installation & Usage

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/sudoku-c.git
   cd sudoku-c
   ```

2. Compile the program:
   ```bash
   gcc -o sudoku main.c play.c settings.c game.c board.c sudokuGenerator.c solver.c -lm
   ```

3. Run the game:
   ```bash
   ./sudoku
   ```

## 🎯 Game Controls

| Command        | Action                          |
|----------------|---------------------------------|
| `A1 5`         | Place number 5 at A1            | 
| `A1 -r`        | Remove number at A1             |                
| `hint`         | Get a hint                      |                
| `save`         | Save current game               |                
| `load`         | Load saved game                 |                
| `exit`         | Quit the game                   |                

## ⚠️ Known Issues

- The remove command allows deleting any cell, even prefilled ones. The game only enforces maintaining at least K values on the board. -> TODO
- Boards (16x16) may not display perfectly on small terminal windows.

## 🧩 Implementation Details

### Board Generation
The game uses a backtracking algorithm to:
1. Fill diagonal boxes first (guaranteed valid configuration)
2. Recursively fill remaining cells with validation
3. Remove numbers to create the puzzle while maintaining a unique solution

### Difficulty Levels
Each level removes a different percentage of numbers:
- Easy: ~45% of cells filled
- Medium: ~30% of cells filled
- Hard: ~20% of cells filled

### Board Sizes
Supported sizes with their block configurations:
- 4x4 (2x2 blocks)
- 9x9 (3x3 blocks)
- 16x16 (4x4 blocks)

## 📊 Statistics Tracking
The game tracks:
- Total solving time
- Number of mistakes
- Hints used

## 🎉 Meme

<img src="https://github.com/user-attachments/assets/2322316c-41f6-4f53-9865-f45adf33d378" width="350" height="250" />

---

Enjoy the game! 🎮
