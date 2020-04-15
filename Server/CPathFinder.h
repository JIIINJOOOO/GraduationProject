#pragma once
#include "node.h"
#define WAY_DIR 4
extern short board[SIDE_LEN][SIDE_LEN];

class CPathFinder {
private:
	list<Node*> openList;
	list<Node*> closedList; 
	list<Position> pathList;
	Position start, end;
	Position direction[WAY_DIR];
	Position current;
	int idx;
public:
	CPathFinder() = default;
	~CPathFinder() = default;

	bool IsOutMap(const Position& pos);
	bool IsOverlap(Node* node);
	list<Position> GetPath(Position start, Position end);
	Node* PathFind(Node* parent, Position end);
};

