#pragma once
#include <iostream>
#include <list>
#include "globals.h"
#include "Physics.h"
using namespace std;

bool Overlap_Start_End(Position s, Position e);
float GetDistance(Position p, Position e);

class Node {
private:
	Position currentPos;
	int costG;	// �Ҹ��� -> ������ Ƚ��
	float costH;	// ������ -> ������������ �Ÿ�
	int costF;	
	Node* parent;
public:
	Node() = default;
	~Node() = default;
	Node(Position pos, Node* parent, int g, Position h);

	int GetCostG() const;
	float GetCostH() const;
	int GetCostF() const;
	Node * GetParent() const;
	Position GetPosition() const;

	void SetCostG(const int& g);
	void SetCostH(Position p);

};