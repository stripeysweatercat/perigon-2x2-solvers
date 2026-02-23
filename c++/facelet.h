#ifndef FACELET
#define FACELET

#include <cstdint>

#define F_U 0
#define F_L 1
#define F_F 2
#define F_R 3
#define F_B 4
#define F_D 5

class Facelet {
public:
	int facelets[24] = {0,0,0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5,5};

	int fromCubeString(char cubestring[]);
};

#endif
