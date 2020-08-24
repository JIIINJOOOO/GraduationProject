#pragma once
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <wingdi.h>
#include "physics.h"
using namespace std;
#define EPSILON 1.0e-10f
#define PIXEL_LENGTH 1531
#define POS_SCALE 100
#define	SIDE_MIN -51000
#define SIDE_MAX 102000
#define POSSIBLE_MOVE_HEIGHT 100

inline bool IsZero(float fValue) { return (fabsf(fValue) < EPSILON); }
inline bool IsEqual(float fa, float fb) { return ::IsZero(fa - fb); }

class CTerrain {
private:
	int m_width;
	int m_height;
	int m_vertexCount;
	int m_indexCount;
	Position m_scale;
	unsigned short* m_mapPixels;
public:
	CTerrain() = default;
	CTerrain(const CTerrain&) = default;
	~CTerrain() {
		if (m_mapPixels) delete[] m_mapPixels;
		m_mapPixels = NULL;
	}

	bool LoadMap(const char* fname);
	void NormalizeMap();
	
	float GetHeight(float x, float y);
	float GetHeightNormal(int x, int y);
	float OnGetHeight(int x, int y);
	unsigned short* GetMap() const;
};

