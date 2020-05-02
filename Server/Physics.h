#pragma once
#include <stdio.h>
#include <math.h>
struct Position {
	float x, y, z;
	Position operator+(const Position& p) {
		Position np;
		np.x = x + p.x;
		np.y = y + p.y;
		np.z = z + p.z;
		return np;
	}
	bool operator==(const Position& p) {
		if ((int)p.x != (int)x) return false;
		if ((int)p.y != (int)y) return false;
		return true;
	}
};

struct Velocity {
	int x, y, z;
};
struct Volume {
	int x, y, z;
};
struct Accel {
	int x, y, z;
};

