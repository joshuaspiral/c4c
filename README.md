# C4C
Connect Four written in C to practice C

# Todo
- [X] Write Connect Four Two Player
- [X] Write AI (https://en.wikipedia.org/wiki/Minimax)
- [X] Write alpha-beta pruning optimisation
- [X] Bitboard optimisation (https://en.wikipedia.org/wiki/Bitboard)
- [X] GUI Game (https://www.raylib.com/ or SDL)
- [ ] Further heuristic optimisations
- [ ] Refactor

# Usage
```
$ make or make ai/ai_bitboard/twoplayer
$ ./twoplayer or ./ai or ./ai_bitboard
```

# Details
./twoplayer - You can play with a friend.  
./ai - The AI implementation I wrote originally. Uses a 2D array of characters to represent the board.  
./ai_bitboard - The AI implementation with pseudo-bitboards. Uses two 1D arrays of 8 bit unsigned integers and evaluated via bitwise logic. (not really a bitboard but still faster than the original implementation).  
./ai_realbitboard - The AI implementation with REAL bitboards. Uses 1 2D array of two 64 bit unsigned integers and evaluated via bitwise logic.

gui/ (Two player SDL implementatino)  
gui-ai/ (AI SDL implementation)  

# Benchmarks
Done on a Ryzen 5 3600 with AI picking first
| Implementation        | Depth | Time (s)  | Visited nodes  |
|-----------------------|-------|-----------|----------------|
| 2D Array Board AI     | 7     | 1.250252  | 6634026        |
| 2D Array Board AI     | 8     | 4.637645  | 46028598       |
| 2D Array Board AI     | 9     | 32.516463 | 314060244      |
| "Pseudo-bitboard" AI  | 7     | 0.171348  | 6602778        |
| "Pseudo-bitboard" AI  | 8     | 1.180703  | 45414144       |
| "Pseudo-bitboard" AI  | 9     | 8.299119  | 310985082      |
| Bitboard AI           | 7     | 0.051276  | 6601433        |
| Bitboard AI           | 8     | 0.384628  | 45386033       |
| Bitboard AI           | 9     | 2.373259  | 310528499      |

Explanation about the game [here](https://toxicfs.xyz/blog/making-a-connect-four-ai/)
