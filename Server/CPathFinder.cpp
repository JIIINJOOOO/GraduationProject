#include "CPathFinder.h"

list<Position> CPathFinder::GetPath(Position start, Position end) {
	// Initialize List
	openList.clear();
	closedList.clear();
	pathList.clear();

	direction[0] = { 1,0,0 };
	direction[1] = { -1,0,0 };
	direction[2] = { 0,1,0 };
	direction[3] = { 0,-1,0 };


	if (!Overlap_Start_End(start, end)) {
		return pathList;
	}
	if (!IsOutMap(end)) {
		return pathList;
	}

	Node* parent = new Node(start, nullptr, 0, end);
	closedList.push_back(parent);

	Node* pathFind = PathFind(parent, end);


	while (pathFind->GetParent() != NULL){
		pathList.insert(pathList.begin(), pathFind->GetPosition());
		pathFind = pathFind->GetParent();
	}
	return pathList;
}

Node* CPathFinder::PathFind(Node* parent, Position end) {
	if (!Overlap_Start_End(parent->GetPosition(), end)) {

		return parent;
	}

	for (int i = 0; i < WAY_DIR; ++i) {
		Position childPos = parent->GetPosition() + direction[i];
		if (IsOutMap(childPos)) {
			if (GetDistance(childPos, end) > parent->GetCostH()) continue;
			Node* newNode = new Node(childPos, parent, parent->GetCostG() + 1, end);
			IsOverlap(newNode);
		}
	}

	Node *solution = openList.front();

	for (auto ol : openList)
		if (solution->GetCostF() >= ol->GetCostF())
			solution = ol;
	
	openList.remove(solution);
	closedList.push_back(solution);

	if (closedList.size() > 1000) return parent;
	return PathFind(solution, end);
}

bool CPathFinder::IsOutMap(const Position& pos) {
	// 좌표는 임시좌표(맵 미완성) 사용
	if (pos.x < -1807 || pos.x> 1007) return false;
	if (pos.y < -1407 || pos.y> 1407) return false;
	int xIdx = (int)pos.x + X_SIDE;
	int yIdx = (int)pos.y + Y_SIDE;
	// z축으로 이동할 수 있는 최대 높이보다 높은가
	// if (board[xIdx][yIdx] > (int)current.z + 20)
	// 	return false;
	return true;
} 

bool CPathFinder::IsOverlap(Node* node) {
	for (auto cl : closedList)
		if (!Overlap_Start_End(cl->GetPosition(), node->GetPosition())) 
			return true;
		
	for (auto ol : openList)
		if (!Overlap_Start_End(ol->GetPosition(), node->GetPosition()))
			if (ol->GetCostF() > node->GetCostF()) {
				openList.remove(ol);
				openList.push_back(node);
				return true;
			}
	openList.push_back(node);
	return true;
}