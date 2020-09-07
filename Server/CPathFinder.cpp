#include "CPathFinder.h"

extern CTerrain *g_tmap;

// list<Position> CPathFinder::GetPath(Position start, Position end) {
list<POS_2D> CPathFinder::GetPath(POS_2D start, POS_2D end) {
	// Initialize List
	auto oIter = openList.begin();
	while (openList.empty() == false) {
		delete[](*oIter);
		openList.erase(oIter);
		oIter = openList.begin();
	}
	auto cIter = closedList.begin();
	while (closedList.empty() == false) {
		delete[](*cIter);
		closedList.erase(cIter);
		cIter = closedList.begin();
	}
	pathList.clear();
	/*
	메모리를 할당하려 넣어줬는데 clear로 메모리를 비워주어서 에러 생겼나?
	할당된 메모리를 각각 명시적으로 삭제해줌
	*/

	direction[X_UP] = { 1,0 };
	direction[X_DOWN] = { -1,0};
	direction[Y_UP]   = { 0,1 };
	direction[Y_DOWN] = { 0,-1 };
	direction[4] = { 1,1 };
	direction[5] = { 1,-1 };
	direction[6] = { -1,1 };
	direction[7] = { -1,-1 };
	// FindPath();
	if (Overlap_Start_End(start, end))
		return pathList;
	
	if (!IsOutMap(end, NULL)) {
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

Node* CPathFinder::PathFind(Node* parent, POS_2D end) {
	if (Overlap_Start_End(parent->GetPosition(), end)) {
		return parent;
	}
	int cnt = 0;
	for (int i = 0; i < WAY_DIR; ++i) {
		POS_2D childPos = parent->GetPosition() + direction[i];
		if (IsOutMap(childPos, i)) {
			// Position p = parent->GetPosition();
			// cout << p.x << "\t" << childPos.x << endl;
			// cout << p.y << "\t" << childPos.y << endl;
			// cout << p.z << "\t" << childPos.z << endl;
			if (parent->GetParent() != nullptr)
				if (childPos == parent->GetParent()->GetPosition())
					continue;
				// if (parent->GetParent()->GetPosition() == childPos) {
				// 	continue;
				// }
			int n = 0;
			// int xIdx = (int)childPos.x + X_SIDE;
			// int yIdx = (int)childPos.y + Y_SIDE;
			// if (board[xIdx][yIdx + 5] - 20 > childPos.z) n = 1;
			// else if (board[xIdx][yIdx - 5] - 20 > childPos.z) n = 2;
			// else if (board[xIdx + 5][yIdx] - 20 > childPos.z) n = 3;
			// else if (board[xIdx - 5][yIdx] - 20 > childPos.z) n = 4;
			
			Node* newNode = new Node(childPos, parent, i, end);
			// if (n > 0) newNode->SetCostH(end, n);
			// Node* newNode = new Node(childPos, parent, parent->GetCostG() + 10, end);
			IsOverlap(newNode);
		}
		else cnt++;
	}

	/*
	4방향중에 한 방향이라도 막혀있는 상태. 즉, cnt가 4보다 WAY_DIR보다 작으면
	한 방향은 막혀있는 상태이다,
	그러면 f값을 조정해줘야하는데
	원래는 단순 거리만으로 평가점수를 만들었는데
	지금은 거리 +@를 해줘야한다. 안그러면 오른쪽으로 이동을 하다가 다시 왼쪽이 더 가까우니까
	그쪽으로 다시 가고 이게 계속 반복된다. 아마도?


	어어 뭔가 생각날거같아
	어떤 축으로 막혔는지를 알아보고 
	그 축에서 빠져나갈 틈을 찾는거야
	예를들어 y축이면 양옆으로 계속 빠져나가서 그 축으로 빠져나갈 구멍이 있는지
	만약 양쪽이 막혀있으면 (그런거 안생기도록 클라랑 협의) 뒤로 가도록
	어쨌든 빠져나갈 구멍이 있으면 거기까지의 거리로 g값을 변경한다.
	*/
	
	Node *solution = openList.front();
	/*
	위의 IsOverlap에서 closedList에서 다 걸려서 빠져나왔는데

	*/
	for (auto ol : openList) {
		if (solution->GetCostF() > ol->GetCostF())
			solution = ol;
		// ol->SetCostG(ol->GetCostG() + 10);
	}
	// if (cnt == 1)
	//  for (auto& ol : openList)
	//  	openList.remove(ol);
	/*
	오케이 지금 문제가 새로 들어오는 노드가 
	빙 돌아서 가야하니까 기존 노드들보다 코스트가 더 높아서 
	기존 노드를 최적해로 선택하는 오류가 나온다.

	지금 문제가 저기서 넣는거까진 상관없는데
	넣고 나서 여기서는 코스트로만 검사하니까
	지금은 코스트가 거리로만 되어있잖아?
	높이 가중치가 적용이 안되서
	경로에 높이차가 있는 장애물이 있으면 
	높이차가 있는건 아는데 가중치 검사에서 이를 고려하지 않아서 
	애가 거기로 가려고만 한다.
	그러면 F비용을 높이까지 고려하도록 다시 짜야한다.
	*/
	openList.remove(solution);
	closedList.push_back(solution);

	if (closedList.size() > MAX_LIST_LEN) return parent;
	return PathFind(solution, end);
}

// bool CPathFinder::IsOutMap(const Position& pos, int direction) {
bool CPathFinder::IsOutMap(const POS_2D& pos, int direction) {
	if (pos.x < SIDE_MIN || pos.x > SIDE_MAX) return false;
	if (pos.y < SIDE_MIN || pos.y > SIDE_MAX) return false;
	int x = static_cast<int>((pos.x + (-SIDE_MIN)) / POS_SCALE);
	int y = static_cast<int>((pos.y + (-SIDE_MIN)) / POS_SCALE);
	// float height = g_tmap->OnGetHeight(x, y);
	// if (height > POSSIBLE_MOVE_HEIGHT) return false;
	return true;
} 

bool CPathFinder::IsOverlap(Node* node) {
	for (auto cl : closedList)
		if (Overlap_Start_End(cl->GetPosition(), node->GetPosition())) {
			return true;
		}
	for (auto ol : openList) {
		if (Overlap_Start_End(ol->GetPosition(), node->GetPosition())) {
			// cout << "ff - " << ol->GetCostF() << "\t" << node->GetCostF() << endl;
			if (ol->GetCostG() > node->GetCostG()) {
				openList.remove(ol);
				node->SetCostG(node->GetParent()->GetCostG() + 10);
				openList.push_back(node);
				return true;
			}

		}
	}
	openList.push_back(node);
	return true;
}
