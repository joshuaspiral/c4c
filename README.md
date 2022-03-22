# C4C
Connect Four written in C to practice C

# Todo
- [X] Write Connect Four Two Player
- [X] Write AI (https://en.wikipedia.org/wiki/Minimax)
- [X] Write alpha-beta pruning optimisation
- [X] Bitboard optimisation (https://en.wikipedia.org/wiki/Bitboard)
- [X] GUI Game (https://www.raylib.com/ or SDL)
- [ ] Further heuristic optimisations
- [X] Refactor

# Usage
`cd` to the directory and run `make` to compile. The executable binaries will be in the same directory.

# Details
./gui/ (Graphical Connect Four written using the [SDL](https://www.libsdl.org/) development library)
./cli/ (Command-line Connect Four written in pure C)

# Benchmarks
A little bit about the implementations.
The latest implementations are using bitboards to store the board position. Here is a benchmark of the different implementations (you can find the older implementations in archive/).
Done on a Ryzen 5 3600 with AI picking first

| Implementation        | Depth | Time (s)  | Visited nodes  |
|-----------------------|-------|-----------|----------------|
| 2D Array Board AI     | 7     | 1.250252  | 6634026        |
| 2D Array Board AI     | 8     | 4.637645  | 46028598       |
| 2D Array Board AI     | 9     | 32.516463 | 314060244      |
| "Pseudo-bitboard" AI  | 7     | 0.171348  | 6602778        |
| "Pseudo-bitboard" AI  | 8     | 1.180703  | 45414144       |
| "Pseudo-bitboard" AI  | 9     | 8.299119  | 310985082      |
| Bitboard AI (latest)  | 7     | 0.051276  | 6601433        |
| Bitboard AI (latest)  | 8     | 0.384628  | 45386033       |
| Bitboard AI (latest)  | 9     | 2.373259  | 310528499      |

Explanation about the development process [here](https://toxicfs.xyz/blog/making-a-connect-four-ai/).
