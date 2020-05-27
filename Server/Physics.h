#pragma once
#include <stdio.h>
#include <math.h>
#include <iostream>
using namespace std;
struct Position {
	float x, y, z;
	Position() = default;
	Position(float a, float b, float c) {
		x = a;
		y = b;
		z = c;
	}
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
	bool Compare(Position p) {
		if ((int)x / 100 != (int)p.x / 100) return false;
		if ((int)y / 100 != (int)p.y / 100) return false;
		return true;
	}
	void Show() const {
		cout << x << "\t" << y << "\t" << z << endl;
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

