#include "cubie.h"
#include "facelet.h"
#include <iostream>

Cubie::Cubie() {	
	//this->genDBLRotTable();
	this->reset();
}

void Cubie::fromFacelet(Facelet f) {
	for (int i = 0; i < 8; i++) {
		int j = 0;
		for (j = 0; j < 3; j++) {
			if (f.facelets[faceletToCubie[i][j]] == 0 || f.facelets[faceletToCubie[i][j]] == 5)
      				break;
		}
		
		int col1 = f.facelets[faceletToCubie[i][(j + 1) % 3]];
		int col2 = f.facelets[faceletToCubie[i][(j + 2) % 3]];

		for (int k = 0; k < 8; k++) {
			if (faceletColour[k][1] == col1 && faceletColour[k][2] == col2) {
				this->cubie[i][0] = k;
				this->cubie[i][1] = j;
			}
		}
	}
}

int Cubie::search(int cost, int bound, int depth) {
	int currentCoord = path[depth];
	int f = cost + DBLHTable[currentCoord];

	if (f > bound)
		return f;

	if (currentCoord == 0)
		return -1;

	int min = 10;

	for (int i = 0; i < 9; i++) {
		int node = DBLRotTable[currentCoord * 9 + i];
		if (node != path[0] && node != path[1] && node != path[2] && node != path[3] && node != path[4]) {
			path[depth + 1] = node;
			moves[depth] = i;
			int sr = search(cost + 1, bound, depth + 1);
			if (sr == -1)
				return -1;
			if (sr < min)
				min = sr;
		}
	}
	return min;
}

void Cubie::solveDBL() {
	path[0] = DBLCoord;
	int bound = DBLHTable[DBLCoord];
	bool solved = false;

	while (!solved) {
		int sr = search(0, bound, 0);

		if (sr == -1) {
			solved = true;
		}
		if (sr == 10) {
			std::cout << "Unable to find solution with depth 2" << std::endl;
		}

		bound = sr;
	}

	for (int i = 0; i < 5; i++) {
		if (moves[i] != 10) {
			applyRot(moves[i]);
		}
	}
	
	printDBLSolution();
}

void Cubie::genDBLRotTable() {
	Cubie temp;

	for (int coord = 0; coord < 24; coord++) {
		for (int rot = 0; rot < 9; rot++) {
			temp.setDBLCoord(coord);
			temp.applyRot(rot);
			temp.calcDBLCoord();

			this->DBLRotTable[coord * 9 + rot] = temp.DBLCoord;
		}
	}
}

void Cubie::genDBLHTable() {
	DBLHTable[0] = 0;

	for (int i = 1; i < 24; i++) {
		bool found = false;

		for (int j = 0; j < 9; j++) {
			if (DBLRotTable[i * 9 + j] == 0) {
				DBLHTable[i] = 1;
				found = true;
			}
		}

		if (!found) {
			DBLHTable[i] = 2;
		}
	}
}

void Cubie::multiply(int cp[], int co[]) {
	int temp_c[8][2];

	for (int i = 0; i < 8; i++) {
		temp_c[i][0] = cubie[cp[i]][0];
		temp_c[i][1] = (cubie[cp[i]][1] + co[i]) % 3;
	}

	for (int i = 0; i < 8; i++) {
		cubie[i][0] = temp_c[i][0];
		cubie[i][1] = temp_c[i][1];
	}
}

void Cubie::applyMove(int m) {
	int i = 0;
	switch (m) {
	case U:
	case U2:
	case UP:
		for (i = 0; i < (m % 3) + 1; i++)
			this->multiply(cp_U, co_U);
		break;
	case R:
	case R2:
	case RP:
		for (i = 0; i < (m % 3) + 1; i++)
			this->multiply(cp_R, co_R);
		break;
	case F:
	case F2:
	case FP:
		for (i = 0; i < (m % 3) + 1; i++)
			this->multiply(cp_F, co_F);
		break;
	default:
		std::cout << "ERROR: Unrecognised move " << m << std::endl;
		break;
	}
}

void Cubie::applyRot(int r) {
	int i = 0;

	switch (r) {
	case X:
	case X2:
	case XP:
		for (i = 0; i < (r % 3) + 1; i++)
			this->multiply(cp_X, co_X);
		break;
	case Y:
	case Y2:
	case YP:
		for (i = 0; i < (r % 3) + 1; i++)
			this->multiply(cp_Y, co_Y);
		break;
	case Z:
	case Z2:
	case ZP:
		for (i = 0; i < (r % 3) + 1; i++)
			this->multiply(cp_Z, co_Z);
		break;
	default:
		std::cout << "ERROR: Unrecognised rotation " << r << std::endl;
		break;
	}

}

void Cubie::calcDBLCoord() {
	int i;
	for (i = 0; i < 8; i++) {
		if (this->cubie[i][0] == DBL)
			break;
	}

	this->DBLCoord = ((7-i) * 3) + cubie[i][1];
}

void Cubie::setDBLCoord(int c) {
	int ori = c % 3;
	int pos = (c - ori) / 3;
	pos = 7 - pos;

	this->reset();

	this->cubie[7][0] = pos;
	this->cubie[7][1] = (3 - ori) % 3;
	this->cubie[pos][0] = 7;
	this->cubie[pos][1] = ori;

	this->DBLCoord = c;
}

void Cubie::printCubie() {
	for (int i = 0; i < 8; i++) {
		std::cout << "[" << this->cubie[i][0] << "," << this->cubie[i][1] << "], ";
	}
	std::cout << "\n";
}

void Cubie::reset() {
	for (int i = 0; i < 8; i++) {
		cubie[i][0] = i;
		cubie[i][1] = 0;
	}
}

void Cubie::printDBLSolution() {
	for (int i = 0; i < 5; i++) {
		if (moves[i] != 10) {
			switch (moves[i]) {
			case 0:
				std::cout << "x ";
				break;
			case 1:
				std::cout << "x2 ";
				break;
			case 2:
				std::cout << "x' ";
				break;
			case 3:
				std::cout << "y ";
				break;
			case 4:
				std::cout << "y2 ";
				break;
			case 5:
				std::cout << "y' ";
				break;
			case 6:
				std::cout << "z ";
				break;
			case 7:
				std::cout << "z2 ";
				break;
			case 8:
				std::cout << "z' ";
				break;
			}
		}
	}
	std::cout << "\n";
}
