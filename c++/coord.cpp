#include "coord.h"
#include <iostream>

int pow(int a, int pow) {
	int b = 1;
	for (int i = 0; i < pow; i++) {
		b *= a;
	}

	return b;
}

int fac(int a) {
	int result = 1;

	for (int i = 1; i < a + 1; i++) {
		result *= i;
	}

	return result;
}

void Coord::fromCubie(Cubie c) {
	// CO - Ternary number built off the orientation of the first 6 corners. DBL is oriented, DBR can only be one value
	this->coCoord = 0;
	for (int i = 0; i < 6; i++) {
		coCoord += c.cubie[i][1] * pow(3,i);
	}

	// CP - weird factorial stuff
	this->cpCoord = 0;
	for (int i = 1; i < 7; i++) {
		int cpVal = c.cubie[i][0];
		int higherCount = 0;
		for (int j = 0; j < i; j++) {
			if (c.cubie[j][0] > cpVal)
				higherCount++;
		}
		cpCoord += higherCount * fac(i);
	}
}

int Coord::getCoCoord(Cubie c) {
	int coord = 0;

	for (int i = 0; i < 6; i++) {
		coord += c.cubie[i][1] * pow(3,i);
	}

	return coord;
}

int Coord::getCpCoord(Cubie c) {
	int coord = 0;

	for (int i = 1; i < 7; i++) {
		int cpVal = c.cubie[i][0];
		int higherCount = 0;
		for (int j = 0; j < i; j++) {
			if (c.cubie[j][0] > cpVal) {
				higherCount++;
			}
		}
		coord += higherCount * fac(i);
	}

	return coord;
}

Cubie Coord::setCoCoord(int c) {
	Cubie r;
	int totalOri = 0;

	r.reset();
	for (int i = 5; i > -1; i--) {
		int ori = 0;

		while (c >= pow(3,i)) {
			c -= pow(3,i);
			ori++;
		}

		r.cubie[i][1] = ori;
		totalOri += ori;
	}

	if (totalOri % 3 != 0) {
		r.cubie[6][1] = 3 - (totalOri % 3);
	}

	return r;
}

Cubie Coord::setCpCoord(int c) {
	Cubie r;

	r.reset();
	int used[7] = {0,0,0,0,0,0,0};
	int order[7] = {0,0,0,0,0,0,0};

	for (int i = 0; i < 7; i++) {
		order[i] = c % (i + 1);
		c = c / (i + 1);
	}

	for (int i = 6; i >=0 ; i--) {
		int k = 6;
		while (used[k] > 0) {
			k--;
		}

		while (order[i] > 0) {
			order[i]--;
			k--;
			while (used[k] > 0) {
				k--;
			}
		}

		r.cubie[i][0] = k;
		used[k] = 1;
	}

	return r;
}

void Coord::genCoMoveTable() {
	Cubie c;

	for (int co = 0; co < 729; co++) {
		for (int move = 0; move < 9; move++) {
			c = setCoCoord(co);
			c.applyMove(move);
			coMoveTable[co * 9 + move] = getCoCoord(c);
		}
	}
}

void Coord::genCpMoveTable() {
	Cubie c;

	int phase2_moveset[5] = {0,1,2,4,7};

	for (int cp = 0; cp < 5040; cp++) {
		for (int move = 0; move < 5; move++) {
			
			c = setCpCoord(cp);
			c.applyMove(phase2_moveset[move]);
			cpMoveTable[cp * 5 + move] = getCpCoord(c);
		}
	}
}

void Coord::traverseCo(int move, int depth) {
	uint16_t oldCoord = coPath[depth - 1];
	uint16_t newCoord = coMoveTable[oldCoord * 9 + move];

	if (coPrune[newCoord] > depth)
		coPrune[newCoord] = depth;
	coPath[depth] = newCoord;

	if (depth < 6) {
		for (int i = 0; i < 9; i++)
			traverseCo(i, depth + 1);
	}
}

void Coord::pruneCoTable() {		// MAXIMUM DEPTH: 6 MOVES
	for (int i = 0; i < 729; i++)
		coPrune[i] = 255;

	coPrune[0] = 0;

	for (int i = 0; i < 9; i++)
		traverseCo(i, 1);

	std::cout << "Finished generating corner orientation pruning table!" << std::endl;

	int missed_co = 0;
	for (int i = 0; i < 729; i++) {
		if (coPrune[i] == 255)
			missed_co++;
	}
}

void Coord::traverseCp(int move, int depth) {		// MAXIMUM DEPTH: 13 MOVES
	uint16_t oldCoord = cpPath[depth - 1];
	uint16_t newCoord = cpMoveTable[oldCoord * 5 + move];

	if (cpPrune[newCoord] > depth)
		cpPrune[newCoord] = depth;
	cpPath[depth] = newCoord;

	if (depth < 13) {
		for (int i = 0; i < 5; i++) 
			traverseCp(i, depth + 1);
	}
}

void Coord::pruneCpTable() {
	for (int i = 0; i < 5040; i++) 
		cpPrune[i] = 255;

	cpPrune[0] = 0;

	for (int i = 0; i < 5; i++) {
		traverseCp(i, 1);
	}

	std::cout << "Finished generating corner permutation pruning table!" << std::endl;

	int missed = 0;
	for (int i = 0; i < 5040; i++) {
		if (cpPrune[i] == 255)
			missed++;
	}
}

void Coord::solve() {
	int raw_sol[21];

	int i = 0;
	int coLength = solveCo();
	for (i = 0; i < coLength; i++)
		raw_sol[i] = coMoves[i];

	int cpLength = solveCp();
	for (i = 0; i < cpLength; i++)
		raw_sol[coLength + i] = cpMoves[i];

	int length = coLength + cpLength;
	bool cancelled = false;

	while (!cancelled) {
		cancelled = true;
		for (int i = 0; i < length - 1; i++) {
			if ((raw_sol[i] / 3) == (raw_sol[i + 1] / 3)) {
				cancelled = false;
				int r = ((raw_sol[i] % 3) + (raw_sol[i + 1] % 3) + 2) % 4;

				if (r == 0) {
					for (int j = i; j < length - 2; j++)
						raw_sol[j] = raw_sol[j + 2];
					length -= 2;
				} else {
					int cancelled_move = (r - 1) + (3 * (raw_sol[i] / 3));
					raw_sol[i] = cancelled_move;

					for (int j = i + 1; j < length - 1; j++)
						raw_sol[j] = raw_sol[j + 1];
					length--;
				}
			}
		}
	}

	printSolution(raw_sol, length);
}

int Coord::solveCo() {
	for (int i = 0; i < 20; i++) {
		coPath[i] = 255;
		coMoves[i] = 255;
	}

	coPath[0] = coCoord;
	int bound = coPrune[coCoord];
	bool solved = false;

	while (!solved) {
		int sr = searchCo(0, bound, 0);

		if (sr == -1)
			solved = true;
		if (sr == 255)
			std::cout << "Unable to find a solution. Retrying..." << std::endl;

		bound = sr;
	}

	// prepare for CP
	Cubie temp;
	int length = 0;

	temp = setCpCoord(cpCoord);
	for (int i = 0; i < 20; i++) {
		if (coMoves[i] != 255) {
			temp.applyMove(coMoves[i]);
			length++;
		}
	}
	cpCoord = getCpCoord(temp);

	return length;
}	

int Coord::searchCo(int cost, int bound, int depth) { 
	int currentCoord = coPath[depth];
	int f = cost + coPrune[currentCoord];

	if (f > bound)
		return f;
	
	if (currentCoord == 0)
		return -1;

	int min = 255;
	for (int i = 0; i < 9; i++) {
		int node = coMoveTable[currentCoord * 9 + i];
		if (isInPath(node, 0) == 0) {
			coPath[depth + 1] = node;
			coMoves[depth] = i;
			int sr = searchCo(cost + 1, bound, depth + 1);
			if (sr == -1)
				return -1;
			if (sr < min)
				min = sr;
		}
	}
	return min;
}

int Coord::solveCp() {
	for (int i = 0; i < 20; i++) {
		cpPath[i] = 255;
		cpMoves[i] = 255;
	}

	cpPath[0] = cpCoord;
	int bound = cpPrune[cpCoord];
	bool solved = false;

	while (!solved) {
		int sr = searchCp(0, bound, 0);

		if (sr == -1)
			solved = true;
		if (sr == 255)
			std::cout << "Unable to find a solution. Retrying..." << std::endl;
		
		bound = sr;
	}
	
	int length = 0;
	for (int i = 0; i < 20; i++) {
		if (cpMoves[i] != 255) {
			length++;
		}
	}

	return length;
}

int Coord::searchCp(int cost, int bound, int depth) { 
	int currentCoord = cpPath[depth];
	int f = cost + cpPrune[currentCoord];

	int phase2_moveset[5] = {0,1,2,4,7};

	if (f > bound)
		return f;
	
	if (currentCoord == 0)
		return -1;

	int min = 255;
	for (int i = 0; i < 5; i++) {
		int node = cpMoveTable[currentCoord * 5 + i];
		if (isInPath(node, 1) == 0) {
			cpPath[depth + 1] = node;
			cpMoves[depth] = phase2_moveset[i];
			int sr = searchCp(cost + 1, bound, depth + 1);
			if (sr == -1)
				return -1;
			if (sr < min)
				min = sr;
		}
	}
	return min;
}

int Coord::isInPath(int val, int cocp) {	// cocp.0 = CO    cocp.1 = CP
	for (int i = 0; i < 20; i++) {
		if (cocp == 0) {
			if (coPath[i] == val)
				return 1;
		} else {
			if (cpPath[i] == val)
				return 1;
		}
	}
	return 0;
}

void Coord::printSolution(int sol[], int len) {
	for (int i = 0; i < len; i++) {
		if (sol[i] != 255) {
			switch (sol[i]) {
			case 0:
				std::cout << "U ";
				break;
			case 1:
				std::cout << "U2 ";
				break;
			case 2:
				std::cout << "U' ";
				break;
			case 3:
				std::cout << "R ";
				break;
			case 4:
				std::cout << "R2 ";
				break;
			case 5:
				std::cout << "R' ";
				break;
			case 6:
				std::cout << "F ";
				break;
			case 7:
				std::cout << "F2 ";
				break;
			case 8:
				std::cout << "F' ";
				break;
			}
		}
	}
	std::cout << "(" << len << ")\n";
}

void Coord::printDistribution() {
	std::cout << "CO Distribution" << std::endl;
	int co_d[7];

	for (int i = 0; i < 7; i++) {
		int count = 0;
		float p = 0;
		for (int j = 0; j < 729; j++) {
			if (coPrune[j] == i)
				count++;
		}

		p = (count / 729.0f) * 100;
		std::cout << i << ": " << count << " (" << p << "%)" << std::endl;
	}

	std::cout << "\n";

	for (int i = 0; i < 14; i++) {
		int count = 0;
		float p = 0;
		for (int j = 0; j < 5040; j++) {
			if (cpPrune[j] == i)
				count++;
		}

		p = (count / 5040.0f) * 100;
		std::cout << i << ": " << count << " (" << p << "%)" << std::endl;
	}
}
