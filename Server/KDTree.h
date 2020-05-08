#pragma once
#include "CObject.h"
#include <iostream>

using namespace std;

#define RIGHT_NODE true
#define LEFT_NODE false

constexpr int max_node{ 100 };
constexpr int xlevel{ 0 };
constexpr int ylevel{ 1 };

class KDTree{
private:
	struct Node {
		Position pos;
		int oid;
		Node *left;
		Node *right;
		Node *parent;
	};

	// ��Ʈ���� ����Ʈ ������Ʈ�� ���� ���߾� ��ǥ�� �Ѵ�.
	Node *m_root = NULL;
	Node *m_head;
public:
	KDTree();
	~KDTree() = default;
	bool Insert(CObject& obj);
	bool Delete(int idx, const Position& pos);
};

