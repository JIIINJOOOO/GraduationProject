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
	�޸𸮸� �Ҵ��Ϸ� �־���µ� clear�� �޸𸮸� ����־ ���� ���峪?
	�Ҵ�� �޸𸮸� ���� ��������� ��������
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
	4�����߿� �� �����̶� �����ִ� ����. ��, cnt�� 4���� WAY_DIR���� ������
	�� ������ �����ִ� �����̴�,
	�׷��� f���� ����������ϴµ�
	������ �ܼ� �Ÿ������� �������� ������µ�
	������ �Ÿ� +@�� ������Ѵ�. �ȱ׷��� ���������� �̵��� �ϴٰ� �ٽ� ������ �� �����ϱ�
	�������� �ٽ� ���� �̰� ��� �ݺ��ȴ�. �Ƹ���?


	��� ���� �������Ű���
	� ������ ���������� �˾ƺ��� 
	�� �࿡�� �������� ƴ�� ã�°ž�
	������� y���̸� �翷���� ��� ���������� �� ������ �������� ������ �ִ���
	���� ������ ���������� (�׷��� �Ȼ��⵵�� Ŭ��� ����) �ڷ� ������
	��·�� �������� ������ ������ �ű������ �Ÿ��� g���� �����Ѵ�.
	*/
	
	Node *solution = openList.front();
	/*
	���� IsOverlap���� closedList���� �� �ɷ��� �������Դµ�

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
	������ ���� ������ ���� ������ ��尡 
	�� ���Ƽ� �����ϴϱ� ���� ���麸�� �ڽ�Ʈ�� �� ���Ƽ� 
	���� ��带 �����ط� �����ϴ� ������ ���´�.

	���� ������ ���⼭ �ִ°ű��� ������µ�
	�ְ� ���� ���⼭�� �ڽ�Ʈ�θ� �˻��ϴϱ�
	������ �ڽ�Ʈ�� �Ÿ��θ� �Ǿ����ݾ�?
	���� ����ġ�� ������ �ȵǼ�
	��ο� �������� �ִ� ��ֹ��� ������ 
	�������� �ִ°� �ƴµ� ����ġ �˻翡�� �̸� ������� �ʾƼ� 
	�ְ� �ű�� ������ �Ѵ�.
	�׷��� F����� ���̱��� ����ϵ��� �ٽ� ¥���Ѵ�.
	*/
	openList.remove(solution);
	closedList.push_back(solution);

	if (closedList.size() > MAX_LIST_LEN) return parent;
	return PathFind(solution, end);
}

bool CPathFinder::IsOutMap(const Position& pos, int direction) {
	// ��ǥ�� �ӽ���ǥ(�� �̿ϼ�) ���
	if (pos.x < -1807 || pos.x> 1007) return false;
	if (pos.y < -1407 || pos.y> 1407) return false;
	int xIdx = (int)pos.x + X_SIDE;
	int yIdx = (int)pos.y + Y_SIDE;
	// z������ �̵��� �� �ִ� �ִ� ���̺��� ������
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
	������ ���������� �������� �¿찡 y��
	���ϰ� x��
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
				cout << "��������" << endl;
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
			// closedList�� �ִ� ���
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
			// closedList�� �ִ� ���
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
			// closedList�� �ִ� ���
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
			// closedList�� �ִ� ���
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