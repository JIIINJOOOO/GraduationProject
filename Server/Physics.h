#pragma once
#include <stdio.h>
#include <math.h>
#include <iostream>
using namespace std;

#define Pi 3.141592

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
	Position operator-(const Position& p) {
		Position np{ x - p.x, y - p.y, z - p.z };
		return np;
	}
	Position operator*(const float& scalar) {
		Position np{ x*scalar, y*scalar, z*scalar };
		return np;
	}
	float DotProduct(const Position& p) {
		return cos(p.x*x + p.y*y + p.z*z);
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
	bool IsZero() const {
		if ((int)x == 0 && (int)y == 0) return true;
		return false;
	}
};

struct Velocity {
	int x, y, z;
};
struct Volume {
	int x, y, z;
	Volume() = default;
	Volume(float a, float b, float c) {
		x = a;
		y = b;
		z = c;
	}
};
struct Accel {
	int x, y, z;
};

