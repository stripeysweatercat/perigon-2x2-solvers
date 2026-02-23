# Perigon's 2x2 Solver Suite
A fun project I started to practice and learn new languages. Not an optimal solver, some stats of the approach below

## Method
The algorith I use is 3 phase: solve DBL with rotations, orient corners, permute corners. CO is bounded at 6 moves, CP (using <U,R2,F2>) is bounded at 13. Given the low number of states after DBL is solved (3,674,160) there is probably merit in just going straight to solved, but part of this project is also to acquaint myself with cube solvers, so splitting into phases with moveset reductions was to help with that goal.

## Goal Languages / Architectures
- [x] C++
- [ ] Rust
- [ ] Python
- [ ] NES/SNES Assembly
- [ ] Web hosted

Any other languages will be added when I feel like it.
