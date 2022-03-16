# C4C
Connect Four written in C to practice C

# Todo
- [X] Write Connect Four Two Player
- [X] Write AI (https://en.wikipedia.org/wiki/Minimax)
- [X] Write alpha-beta pruning optimisation
- [ ] Further heuristic optimisations
- [X] Bitboard optimisation (https://en.wikipedia.org/wiki/Bitboard)
- [ ] GUI Game (https://www.raylib.com/ or SDL)

# Usage
```
$ ./build.sh
$ ./twoplayer or ./ai or ./ai_bitboard
```

# Details
./twoplayer - You can play with a friend.  
./ai - The AI implementation I wrote originally. Uses a 2D array of characters to represent the board.  
./ai_bitboard - The AI implementation with bitboards. Uses two 1D arrays of 8 bit unsigned integers and evaluated via bitwise logic. (not really a bitboard but still faster than the original implementation).  

# Benchmarks
Done on a Ryzen 5 3600 with AI picking first
| Implementation | Depth | Time (s)  |
|----------------|-------|-----------|
| AI             | 9     | 32.516463 |
| Bitboard AI    | 9     | 8.217365  |
| AI             | 8     | 4.637645  |
| Bitboard AI    | 8     | 1.228031  |
