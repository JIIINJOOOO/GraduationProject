#include "CPathFinder.h"
#include "CTerrain.h"

extern CTerrain *g_tmap;

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
				if (childPos.Compare(parent->GetParent()->GetPosition()))
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
	// if (pos.x < -1807 || pos.x> 1007) return false;
	// if (pos.y < -1407 || pos.y> 1407) return false;
	// int xIdx = (int)pos.x + X_SIDE;
	// int yIdx = (int)pos.y + Y_SIDE;
	/*
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
	*/
	// if (board[xIdx][yIdx]-30 > pos.z) return false;
	// return true;

	if (pos.x < SIDE_MIN || pos.x > SIDE_MAX) return false;
	if (pos.y < SIDE_MIN || pos.y > SIDE_MAX) return false;
	int x = static_cast<int>((pos.x + (-SIDE_MIN)) / POS_SCALE);
	int y = static_cast<int>((pos.y + (-SIDE_MIN)) / POS_SCALE);
	float height = g_tmap->OnGetHeight(x, y) - pos.z;
	if (height > POSSIBLE_MOVE_HEIGHT) return false;
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
