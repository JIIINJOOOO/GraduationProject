#include "node.h"
short board[SIDE_LEN][SIDE_LEN];
bool Overlap_Start_End(POS_2D s, POS_2D e) {
	if (s == e) return true;
	return false;
}

float GetDistance(Position p, Position e) {
	int x = pow((p.x - e.x), 2);
	int y = pow((p.y - e.y), 2);
	// int z = pow((p.z - e.z), 2);
	float distance = sqrt(x + y);
	return abs(distance);
}
bool IsBlock(Position p) {
	if (board[(int)p.x+1][(int)p.y] - 20 > p.z) return true;
	if (board[(int)p.x-1][(int)p.y] - 20 > p.z) return true;
	if (board[(int)p.x][(int)p.y+1] - 20 > p.z) return true;
	if (board[(int)p.x][(int)p.y-1] - 20 > p.z) return true;
	return false;
}
//Node::Node(Position pos, Node* parent, int g, Position h) {
//	currentPos = pos;
//	this->parent = parent;
//	endPos = h;
//
//	// �ӽ÷� g �ڸ��� ������ �޾Ƽ� ó���غ���
//	if (parent == nullptr) costG = 0;
//	else if (g == X_UP || g == X_DOWN || g == Y_UP || g == Y_DOWN)
//		costG = this->parent->GetCostG() + 10;
//	else
//		costG = this->parent->GetCostG() + 14;
//
//	SetCostH(h);
//}

Node::Node(POS_2D pos, Node* parent, int dir, POS_2D h) {
	currentPos = pos;
	this->parent = parent;
	endPos = h;

	if (parent == NULL) costG = 0;
	else if (dir == X_UP || dir == X_DOWN || dir == Y_UP || dir == Y_DOWN)
		costG = this->parent->GetCostG() + 10;
	else costG = this->parent->GetCostG() + 14;
	SetCostH(h);
}

int Node::GetCostG() const {
	return costG;
}

float Node::GetCostH() const {
	return costH;
}

float Node::GetCostF() const {
	return ((float)costG + costH);
}

Node* Node::GetParent() const {
	return parent;
}

POS_2D Node::GetPosition() const {
	return currentPos;
	// return currentPos;
}

void Node::SetParent(Node* n) {
	parent = n;
}


void Node::SetCostG(const int& g) {
	costG = g;
}

void Node::SetCostH(Position p, const int block) {
	/*int xIdx = (int)currentPos.x + X_SIDE;
	int yIdx = (int)currentPos.y + Y_SIDE;

	for (int i = 1; i < 1000; ++i)
		if (block == 1) {
			if (board[xIdx - i][yIdx + 5] - 20 < currentPos.z ||
				board[xIdx + i][yIdx + 5] - 20 < currentPos.z) {
				costH = i + static_cast<int>(abs(endPos.y - currentPos.y));
				break;
			}
		}
		else if (block == 2) {
			if (board[xIdx - i][yIdx - 5] - 20 < currentPos.z ||
				board[xIdx + i][yIdx - 5] - 20 < currentPos.z) {
				costH = i + static_cast<int>(abs(endPos.y - currentPos.y));
				break;
			}
		}
		else if (block == 3) {
			if (board[xIdx + 5][yIdx - i] - 20 < currentPos.z ||
				board[xIdx + 5][yIdx + i] - 20 < currentPos.z) {
				costH = i + static_cast<int>(abs(endPos.x - currentPos.x));
				break;
			}
		}
		else if (block == 4) {
			if (board[xIdx - 5][yIdx - i] - 20 < currentPos.z ||
				board[xIdx - 5][yIdx + i] - 20 < currentPos.z) {
				costH = i + static_cast<int>(abs(endPos.x - currentPos.x));
				break;
			}
		}*/
	
	// �̰Ÿ�
	// costH = (abs(p.x - currentPos.x) + abs(p.y - currentPos.y));

}

void Node::SetCostH(POS_2D p) {
	auto x = static_cast<unsigned short>(abs(p.x - currentPos.x));
	auto y = static_cast<unsigned short>(abs(p.y - currentPos.y));
	costH = (x + y) * 10;
}

void Node::SetCostH(Position p) {
	// costH = GetDistance(currentPos, p);
	int x = static_cast<int>(abs(p.x - currentPos.x));
	int y = static_cast<int>(abs(p.y - currentPos.y));
	costH = (abs(p.x - currentPos.x) + abs(p.y - currentPos.y)) * 10;
	
	


	// ....---...
	/*
	���⼭ ���̸� ����ؾ� �Ѵٶ�...
	�߰������� ����ؾ��ϴµ�
	�׷����� �Ÿ��δ� �������
	��ġ? �׷�����
	�ƴ� �ٵ� �� �տ� ��ֹ��� �ִµ� �װ� ������ �̵��ص� �Ȱ��ݾ� �Ÿ��� 1���̴ϱ� ����
	������ �̵��ص� 1���̴ϱ� ���̰� ���̳���
	���� �� �յ� ������ ���� �������̶����� ��������
	�׷��ϱ� ��� �������� ��������
	�̰� ����?
	�Ƹ� �׷��ž�

	���� �ٽ� ���ϱ�
	������ ��� �͸����� �ϰ��־� ������
	�������� ���µ� �������� �� ���°� �ƴ϶� ���� �����ʿ� �����ϱ� �ٽ� ���������� ���ž�
	�ٵ� �����ʿ��� ������ �Ÿ��� �־����ϱ� �ٽ� �������� �̵��� ��
	�̰� ��ӹݺ��Ǵ°ž�
	��� �͸����� �ϴ°���
	*/
}