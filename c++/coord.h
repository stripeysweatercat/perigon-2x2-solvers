#ifndef COORD
#define COORD

#include "cubie.h"
#include <cstdint>

class Coord {
public:
	int coCoord = 0;
	int cpCoord = 0;

	void fromCubie(Cubie c);

	int getCoCoord(Cubie c);
	int getCpCoord(Cubie c);

	Cubie setCoCoord(int c);
	Cubie setCpCoord(int c);

	uint16_t coMoveTable[729 * 9];
	uint16_t cpMoveTable[5040 * 5];

	void genCoMoveTable();
	void genCpMoveTable();

	uint8_t coPrune[729];
	uint8_t cpPrune[5040];
	int coPath[20];
	int cpPath[20];

	void peekCpPrune(int coord);

	void pruneCoTable();
	void traverseCo(int move, int depth);
	void pruneCpTable();
	void traverseCp(int move, int depth);

	void solve();

	int solveCo();
	int searchCo(int cost, int bound, int depth);
	int coMoves[20];

	int solveCp();
	int searchCp(int cost, int bound, int depth);
	int cpMoves[20];

	int isInPath(int val, int cocp);
	void printSolution(int sol[], int len);
	void printDistribution();
};

#endif
