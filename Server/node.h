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
bool Overlap_Start_End(Position s, Position e);
float GetDistance(Position p, Position e);
extern short board[SIDE_LEN][SIDE_LEN];

class Node {
private:
	Position currentPos;
	Position endPos;
	int costG;	// �Ҹ��� -> ������ Ƚ��
	float costH;	// ������ -> ������������ �Ÿ�
	float costF;	
	Node* parent;
public:
	Node() = default;
	~Node() = default;
	Node(Position pos, Node* parent, int g, Position h);

	int GetCostG() const;
	float GetCostH() const;
	float GetCostF() const;
	Node * GetParent() const;
	Position GetPosition() const;

	void SetCostG(const int& g);
	void SetCostH(Position p);
	void SetCostH(Position p, const int h);
	void SetParent(Node* n);
};