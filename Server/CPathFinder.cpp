#include "CPathFinder.h"

list<Position> CPathFinder::GetPath(Position start, Position end) {
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

	direction[X_UP]   = { 1,0,0 };
	direction[X_DOWN] = { -1,0,0};
	direction[Y_UP]   = { 0,1,0 };
	direction[Y_DOWN] = { 0,-1,0 };
	// direction[4] = { 1,1,0 };
	// direction[5] = { 1,-1,0 };
	// direction[6] = { -1,1,0 };
	// direction[7] = { -1,-1,0 };
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

Node* CPathFinder::PathFind(Node* parent, Position end) {
	if (Overlap_Start_End(parent->GetPosition(), end)) {
		return parent;
	}
	int cnt = 0;
	for (int i = 0; i < WAY_DIR; ++i) {
		Position childPos = parent->GetPosition() + direction[i];
		if (IsOutMap(childPos, i)) {
			

			// Position p = parent->GetPosition();
			// cout << p.x << "\t" << childPos.x << endl;
			// cout << p.y << "\t" << childPos.y << endl;
			// cout << p.z << "\t" << childPos.z << endl;
			if (parent->GetParent() != nullptr)
				if (parent->GetParent()->GetPosition() == childPos) {
					continue;
				}
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
	if (cnt == 1)
	 for (auto& ol : openList)
	 	openList.remove(ol);
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

bool CPathFinder::IsOutMap(const Position& pos, int direction) {
	// 좌표는 임시좌표(맵 미완성) 사용
	if (pos.x < -1807 || pos.x> 1007) return false;
	if (pos.y < -1407 || pos.y> 1407) return false;
	int xIdx = (int)pos.x + X_SIDE;
	int yIdx = (int)pos.y + Y_SIDE;
	// z축으로 이동할 수 있는 최대 높이보다 높은가
	// if (board[xIdx][yIdx]-20 > ((int)pos.z))
	// 	return false;
	// if (direction == Y_UP)
	// 	if (board[xIdx][yIdx + 10] - 50 > pos.z) {
	// 		return false;
	// 	}
	// if (board[xIdx][yIdx+10] - 50 > pos.z) return false;
	// if (board[xIdx][yIdx-10] - 50 > pos.z) return false;
	// if (board[xIdx+10][yIdx] - 50 > pos.z) return false;
	// if (board[xIdx-10][yIdx] - 50 > pos.z) return false;
	// if (direction == Y_UP && board[xIdx][yIdx + 10] - 20 > pos.z) return false;
	// if (direction == Y_DOWN && board[xIdx][yIdx - 10] - 20 > pos.z) return false;
	// if (direction == X_UP && board[xIdx+10][yIdx] - 20 > pos.z) return false;
	// if (direction == X_DOWN && board[xIdx-10][yIdx] - 20 > pos.z) return false;

	if (board[xIdx][yIdx]-30 > pos.z) return false;
	return true;

	/*
	위에서 정방향으로 보았을때 좌우가 y축
	상하가 x축
	*/
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
				cout << "ㅁㄴㅇㄹ" << endl;
				node->SetCostG(node->GetParent()->GetCostG() + 10);
				openList.push_back(node);
				return true;
			}

		}
	}
	openList.push_back(node);
	return true;
}
/*
//////////////////////////////////////////////////////////
list<Position> CPathFinder::FindPath(Position start, Position end) {
	Node* selectedNode;

	Node * node = new Node(start, NULL, NULL, end);	
	openList.push_back(node);

	auto iter = openList.begin();
	while ((openList.end() != openList.begin()) && (openList.end() == FindPosNode(end, &openList))) {
		iter = FindNextNode(&openList);
		selectedNode = *iter;

		ExploreNode(selectedNode, &openList, &closedList, end);

		closedList.push_back(selectedNode);
		openList.erase(iter);
	}

	// Success search 
	if (openList.end() != openList.begin()) {
		iter = FindPosNode(end, &openList);
		Node*& s = selectedNode;
		for (s = *iter; s->GetParent() != NULL; s = s->GetParent())
			pathList.push_back(s->GetPosition());
		pathList.push_back(s->GetPosition());
		pathList.reverse();

		iter = openList.begin();
		for (; iter != openList.end(); ++iter)
			delete *iter;
		iter = closedList.begin();
		for (; iter != closedList.end(); ++iter)
			delete *iter;
		return pathList;
	}

	// Fail search
	iter = closedList.begin();
	for (; iter != closedList.end(); ++iter)
		delete *iter;
	return pathList;
}

void CPathFinder::ExploreNode(Node* node, list<Node*>* o_node, list<Node*>* c_node, Position end) {
	list<Node*>::iterator iter;
	Position pos;
	bool up = true, right = true, down = true, left = true;

	pos.x = static_cast<int>(node->GetPosition().x) - 1;
	pos.y = static_cast<int>(node->GetPosition().y);
	if (IsOutMap(pos)) {
		up = false;
		if (openList.end() != FindPosNode(pos, &openList)) {
			iter = FindPosNode(pos, &openList);
			if ((*iter)->GetCostG() > node->GetCostG() + 10)
				(*iter)->SetParent(node);
		}
		else if (closedList.end() != FindPosNode(pos, &closedList)) {
			// closedList에 있는 경우
		}
		else {
			Node *n = new Node(pos, node, NULL, end);
			openList.push_back(n);
		}
	}

	pos.x = static_cast<int>(node->GetPosition().x);
	pos.y = static_cast<int>(node->GetPosition().y) + 1;
	if (IsOutMap(pos)) {
		right = false;
		if (openList.end() != FindPosNode(pos, &openList)) {
			iter = FindPosNode(pos, &openList);
			if ((*iter)->GetCostG() > node->GetCostG() + 10)
				(*iter)->SetParent(node);
		}
		else if (closedList.end() != FindPosNode(pos, &closedList)) {
			// closedList에 있는 경우
		}
		else {
			Node *n = new Node(pos, node, NULL, end);
			openList.push_back(n);
		}
	}

	pos.x = static_cast<int>(node->GetPosition().x)+1;
	pos.y = static_cast<int>(node->GetPosition().y);
	if (IsOutMap(pos)) {
		down = false;
		if (openList.end() != FindPosNode(pos, &openList)) {
			iter = FindPosNode(pos, &openList);
			if ((*iter)->GetCostG() > node->GetCostG() + 10)
				(*iter)->SetParent(node);
		}
		else if (closedList.end() != FindPosNode(pos, &closedList)) {
			// closedList에 있는 경우
		}
		else {
			Node *n = new Node(pos, node, NULL, end);
			openList.push_back(n);
		}
	}

	pos.x = static_cast<int>(node->GetPosition().x);
	pos.y = static_cast<int>(node->GetPosition().y)-1;
	if (IsOutMap(pos)) {
		left = false;
		if (openList.end() != FindPosNode(pos, &openList)) {
			iter = FindPosNode(pos, &openList);
			if ((*iter)->GetCostG() > node->GetCostG() + 10)
				(*iter)->SetParent(node);
		}
		else if (closedList.end() != FindPosNode(pos, &closedList)) {
			// closedList에 있는 경우
		}
		else {
			Node *n = new Node(pos, node, NULL, end);
			openList.push_back(n);
		}
	}

	pos.x = static_cast<int>(node->GetPosition().x) - 1;
	pos.y = static_cast<int>(node->GetPosition().y) + 1;
	if (IsOutMap(pos) && !up && !right) {
		if (openList.end() != FindPosNode(pos, &openList));
		if ((*iter)->GetCostG() > node->GetCostG() + 14)
			(*iter)->SetParent(node);
		else if (closedList.end() != FindPosNode(pos, &closedList)) {

		}
		else {
			Node *n = new Node(pos, node, NULL, end);
			openList.push_back(n);
		}
	}

	pos.x = static_cast<int>(node->GetPosition().x) + 1;
	pos.y = static_cast<int>(node->GetPosition().y) + 1;
	if (IsOutMap(pos) && !down && !right) {
		if (openList.end() != FindPosNode(pos, &openList));
		if ((*iter)->GetCostG() > node->GetCostG() + 14)
			(*iter)->SetParent(node);
		else if (closedList.end() != FindPosNode(pos, &closedList)) {

		}
		else {
			Node *n = new Node(pos, node, NULL, end);
			openList.push_back(n);
		}
	}

	pos.x = static_cast<int>(node->GetPosition().x) + 1;
	pos.y = static_cast<int>(node->GetPosition().y) - 1;
	if (IsOutMap(pos) && !down && !left) {
		if (openList.end() != FindPosNode(pos, &openList));
		if ((*iter)->GetCostG() > node->GetCostG() + 14)
			(*iter)->SetParent(node);
		else if (closedList.end() != FindPosNode(pos, &closedList)) {

		}
		else {
			Node *n = new Node(pos, node, NULL, end);
			openList.push_back(n);
		}
	}

	pos.x = static_cast<int>(node->GetPosition().x) - 1;
	pos.y = static_cast<int>(node->GetPosition().y) - 1;
	if (IsOutMap(pos) && !up && !left) {
		if (openList.end() != FindPosNode(pos, &openList));
		if ((*iter)->GetCostG() > node->GetCostG() + 14)
			(*iter)->SetParent(node);
		else if (closedList.end() != FindPosNode(pos, &closedList)) {

		}
		else {
			Node *n = new Node(pos, node, NULL, end);
			openList.push_back(n);
		}
	}
}
                               
list<Node*>::iterator CPathFinder::FindNextNode(list<Node*>* o_node) {
	auto iter = openList.begin();
	int minValue = 200'000'000;
	int order = 0;

	for (int i = 1; iter != openList.end(); ++i, ++iter)
		if ((*iter)->GetCostF() <= minValue) {
			minValue = (*iter)->GetCostF();
			order = i;
		}
	iter = openList.begin();
	for (int i = 1; i < order; ++i)
		++iter;
	return iter;
}

list<Node*>::iterator CPathFinder::FindPosNode(Position pos, list<Node*>* nodeList) {
	auto iter = openList.begin();
	for (int i = 1; iter != openList.end(); ++i, ++iter)
		if ((*iter)->GetPosition() == pos) return iter;
	return openList.end();
}
void CPathFinder::FindPath() {
	pathList = FindPath(start, end);
	iter = pathList.begin();
}
Position CPathFinder::GetPosition(const int n) {
	for (int i = 1; i < n; ++i)
		++iter;
	Position p = (*iter);
	iter = pathList.begin();
	return p;
}
*/