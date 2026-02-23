#include <iostream>
#include "facelet.h"
#include "cubie.h"
#include "coord.h"

Facelet f;
Cubie c;
Coord coord;

int main(int argc, char* argv[])
{
	if (argc == 1)
		std::cout << "USAGE: 222solve [cubestring]" << std::endl;
	else {
		f.fromCubeString(argv[1]);
		c.fromFacelet(f);

		c.genDBLRotTable();
		c.genDBLHTable();
		coord.genCoMoveTable();
		coord.genCpMoveTable();

		coord.pruneCoTable();
		coord.pruneCpTable();

		//solving
		c.calcDBLCoord();
		c.solveDBL();

		coord.fromCubie(c);
	
		coord.solve();
	}
	return 0;
}
