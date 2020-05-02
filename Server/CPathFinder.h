#pragma once
#include "node.h"
#define WAY_DIR 4


#define MAX_LIST_LEN 1000

class CPathFinder {
private:
	list<Node*> openList;
	list<Node*> closedList; 
	list<Position> pathList;
	Position start, end;
	Position direction[WAY_DIR];
	// Position current;
	int idx;

	// A* 2
	list<Position>::iterator iter;

public:
	CPathFinder() = default;
	~CPathFinder() = default;

	bool IsOutMap(const Position& pos, int direction);
	bool IsOverlap(Node* node);
	list<Position> GetPath(Position start, Position end);
	Node* PathFind(Node* parent, Position end);

	// Test
	//void FindPath();
	//Position GetPosition(const int n);
	//list<Position> FindPath(Position start, Position end);
	//list<Node*>::iterator FindNextNode(list<Node*>* o_node);
	//list<Node*>::iterator FindPosNode(Position pos, list<Node*>* nodeList);
	//void ExploreNode(Node* node, list<Node*>* o_node, list<Node*>* c_node, Position end);
};

