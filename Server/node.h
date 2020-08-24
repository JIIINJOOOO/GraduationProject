#pragma once
#include <iostream>
#include <list>
#include "globals.h"
#include "Physics.h"
using namespace std;

#define X_UP 0
#define X_DOWN 1
#define Y_UP 2
#define Y_DOWN 3
constexpr int Xdd = 0;
float GetDistance(Position p, Position e);
// short board[SIDE_LEN][SIDE_LEN];

struct POS_2D {
	short x, y;
	bool operator==(const POS_2D& a) {
		if (x != a.x) return false;
		if (y != a.y) return false;
		return true;
	}
	POS_2D operator+(const POS_2D& a) {
		POS_2D p;
		p.x = x + a.x;
		p.y = y + a.y;
		return p;
	}
};

bool Overlap_Start_End(POS_2D s, POS_2D e);
bool Overlap_Start_End(POS_2D s, POS_2D e);
class Node {
private:
	// Position currentPos;
	POS_2D currentPos;
	// Position endPos;
	POS_2D endPos;
	int costG;	// 소모비용 -> 움직인 횟수
	float costH;	// 예상비용 -> 도착점까지의 거리
	float costF;	
	Node* parent;
public:
	Node() = default;
	~Node() = default;
	// Node(Position pos, Node* parent, int g, Position h);
	Node(POS_2D pos, Node* parent, int dir, POS_2D h);

	int GetCostG() const;
	float GetCostH() const;
	float GetCostF() const;
	Node * GetParent() const;
	POS_2D GetPosition() const;

	void SetCostG(const int& g);
	void SetCostH(Position p);
	void SetCostH(POS_2D p);
	void SetCostH(Position p, const int h);
	void SetParent(Node* n);
};