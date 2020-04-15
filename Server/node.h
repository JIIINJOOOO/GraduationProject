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
	int costG;	// 소모비용 -> 움직인 횟수
	float costH;	// 예상비용 -> 도착점까지의 거리
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