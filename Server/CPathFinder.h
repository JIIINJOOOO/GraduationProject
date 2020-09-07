#pragma once
#include "CTerrain.h"
#include "node.h"
#define WAY_DIR 8


#define MAX_LIST_LEN 1000

class CPathFinder {
private:
	list<Node*> openList;
	list<Node*> closedList; 
	// list<Position> pathList;
	POS_2D start, end;
	POS_2D direction[WAY_DIR];
	int idx;

	// POS_2D
	list<POS_2D> pathList;


	// A* 2
	// list<POS_2D>::iterator iter;

public:
	CPathFinder() = default;
	~CPathFinder() = default;

	
	// bool IsOutMap(const Position& pos, int direction);
	bool IsOutMap(const POS_2D& pos, int direction);
	bool IsOverlap(Node* node);
	// list<Position> GetPath(Position start, Position end);
	list<POS_2D> GetPath(POS_2D start, POS_2D end);

	Node* PathFind(Node* parent, POS_2D end);

	// Test
	//void FindPath();
	//Position GetPosition(const int n);
	//list<Position> FindPath(Position start, Position end);
	//list<Node*>::iterator FindNextNode(list<Node*>* o_node);
	//list<Node*>::iterator FindPosNode(Position pos, list<Node*>* nodeList);
	//void ExploreNode(Node* node, list<Node*>* o_node, list<Node*>* c_node, Position end);
};

