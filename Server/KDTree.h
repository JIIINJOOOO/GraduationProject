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

	// 루트노드는 디폴트 오브젝트로 맵의 정중앙 좌표로 한다.
	Node *m_root = NULL;
	Node *m_head;
public:
	KDTree();
	~KDTree() = default;
	bool Insert(CObject& obj);
	bool Delete(int idx, const Position& pos);
};

