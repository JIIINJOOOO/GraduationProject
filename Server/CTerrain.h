#pragma once
#include <iostream>
#include <fstream>
#include <gl/freeglut.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#include "physics.h"
using namespace std;

class CTerrain {
private:
	struct Color {
		float x, y, z, w;
		Color() = default;
		Color(float a, float b, float c, float d) {
			x = a; y = b; z = c; w = d;
		}
	};
	struct Vertex {
		Position pos;
		Color color;
	};
private:
	int m_width;
	int m_height;
	int m_vertexCount;
	int m_indexCount;
	Position* m_map;
	Vertex* m_vertex;
public:
	CTerrain() = default;
	CTerrain(const CTerrain&) = default;
	~CTerrain() = default;;

	bool LoadMap(const char* fname);
	void NormalizeMap();
	bool InitializeBuffers();

};

