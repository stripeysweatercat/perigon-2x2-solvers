#ifndef CUBIE
#define CUBIE

#include "facelet.h"
#include <cstdint>

#define UBL 0
#define UBR 1
#define UFR 2
#define UFL 3
#define DFL 4
#define DFR 5
#define DBR 6
#define DBL 7

#define U 0
#define U2 1
#define UP 2
#define R 3
#define R2 4
#define RP 5
#define F 6
#define F2 7
#define FP 8

#define X 0
#define X2 1
#define XP 2
#define Y 3
#define Y2 4
#define YP 5
#define Z 6
#define Z2 7
#define ZP 8

class Cubie {
public:
	Cubie();

	int cubie[8][2] = {{0,0},{1,0},{2,0},{3,0},{4,0},{5,0},{6,0},{7,0}};
	int DBLCoord = 0;

	// moves
	int cp_U[8] = {UFL, UBL, UBR, UFR, DFL, DFR, DBR, DBL};
	int cp_R[8] = {UBL, UFR, DFR, UFL, DFL, DBR, UBR, DBL};
	int cp_F[8] = {UBL, UBR, UFL, DFL, DFR, UFR, DBR, DBL};

	int co_U[8] = {0,0,0,0,0,0,0,0};
	int co_R[8] = {0,1,2,0,0,1,2,0};
	int co_F[8] = {0,0,1,2,1,2,0,0};

	//rotations
	int cp_X[8] = {UFL, UFR, DFR, DFL, DBL, DBR, UBR, UBL};
	int cp_Y[8] = {UFL, UBL, UBR, UFR, DFR, DBR, DBL, DFL};
	int cp_Z[8] = {DBL ,UBL, UFL, DFL, DFR, UFR, UBR, DBR};

	int co_X[8] = {2,1,2,1,2,1,2,1};
	int co_Y[8] = {0,0,0,0,0,0,0,0};
	int co_Z[8] = {1,2,1,2,1,2,1,2};


	int faceletToCubie[8][3] = {{0,4,17},{1,16,13},{2,12,9},{3,8,5},{20,6,11},{21,10,15},{22,14,19},{23,18,7}};

	int faceletColour[8][3] = {{0,1,4},{0,4,3},{0,3,2},{0,2,1},{5,1,2},{5,2,3},{5,3,4},{5,4,1}};

	void fromFacelet(Facelet f);
	void printCubie();
	void reset();

	void multiply(int cp[], int co[]);
	void applyMove(int m);
	void applyRot(int r);

	void calcDBLCoord();
	void setDBLCoord(int c);

	uint8_t DBLRotTable[24 * 9];
	uint8_t DBLHTable[24 * 9];

	void genDBLRotTable();
	void genDBLHTable();

	void solveDBL();
	int search(int cost, int bound, int depth);
	int path[6] = {25, 25, 25, 25, 25, 25};
	int moves[5] = {10, 10, 10, 10, 10};

	void printDBLSolution();
};

#endif
