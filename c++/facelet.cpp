#include <iostream>
#include "facelet.h"

int Facelet::fromCubeString(char cubestring[]) {
	int faceletCount[6] = {0,0,0,0,0,0};

	for (int i = 0; i < 24; i++) {
		switch (cubestring[i]) {
		case 'U':
			facelets[i] = 0;
			faceletCount[0]++;
			break;
		case 'L':
			facelets[i] = 1;
			faceletCount[1]++;
			break;
		case 'F':
			facelets[i] = 2;
			faceletCount[2]++;
			break;
		case 'R':
			facelets[i] = 3;
			faceletCount[3]++;
			break;
		case 'B':
			facelets[i] = 4;
			faceletCount[4]++;
			break;
		case 'D':
			facelets[i] = 5;
			faceletCount[5]++;
			break;
		default:
			std::cout << "Unexpected character " << facelets[i] << std::endl;
			return -1;
		}
	}

	for (int i = 0; i < 6; i++) {
		if (faceletCount[i] > 4) {
			std::cout << "ERROR: Too many " << i << " facelets" << std::endl;
			return -1;
		} else if (faceletCount[i] < 4) {
			std::cout <<  "ERROR: TOO few " << i << " facelets" << std::endl;
			return -1;
		}
	}

	return 0;
}
