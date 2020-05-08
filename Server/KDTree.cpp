#include "KDTree.h"

KDTree::KDTree() {
	// Create root node
	m_root = new Node;
	m_root->pos = { 0.f, 0.f, 0.f };
	m_root->parent = NULL;
	m_root->right = NULL;
	m_root->left = NULL;
}

bool KDTree::Insert(CObject& obj) {
	// Insert New Node (except root node)
	if (m_root == NULL) return false;
	int level = xlevel;
	Node *newNode = new Node;
	newNode->pos = obj.GetPosition();
	newNode->right = NULL;
	newNode->left = NULL;
	Node *parent = new Node;
	parent = m_root;
	m_head = m_root;

	while (true) {
		parent = m_head;
		if (level == xlevel) {
			if (m_head->pos.x < newNode->pos.x)
				m_head = m_head->right;
			else m_head = m_head->left;
		}
		else if (level == ylevel) {
			if (m_head->pos.y < newNode->pos.y)
				m_head = m_head->right;
			else m_head = m_head->left;
		}
		if (m_head == NULL) break;
		level = (level + 1) % 2;
	}

	if (level == xlevel) {
		if (parent->pos.x < newNode->pos.x)
			parent->right = newNode;
		else parent->left = newNode;
	}
	else if (level == ylevel) {
		if (parent->pos.y < newNode->pos.y)
			parent->right = newNode;
		else parent->left = newNode;
	}
	newNode->parent = parent;
	parent = NULL;
	m_head = NULL;
	cout << "Level : " << level << "\t";
	cout << newNode->pos.x << " " << newNode->pos.y << endl;
	return true;
}

bool KDTree::Delete(int idx, const Position& pos) {
	// Delete 
	if (m_root == NULL) return false;
	m_head = m_root;
	int level = xlevel;
	bool direction = NULL;

	while (true) {
		if (m_head == NULL) break;
		if (m_head->oid == idx) break;

		if (level == xlevel) {
			if (m_head->pos.x < pos.x) {
				m_head = m_head->right;
				direction = RIGHT_NODE;
			}
			else {
				m_head = m_head->left;
				direction = LEFT_NODE;
			}
		}
		else if (level == ylevel) {
			if (m_head->pos.y < pos.y) {
				m_head = m_head->right;
				direction = RIGHT_NODE;
			}
			else {
				m_head = m_head->left;
				direction = LEFT_NODE;
			}
		}
		level = (level + 1) % 2;
	}
	if (m_head == NULL) return false;

	Node*& parent = m_head->parent;
	if (m_head->left == NULL && m_head->right == NULL) {
		if (direction == RIGHT_NODE)
			m_head->parent->right = NULL;
		else m_head->parent->left = NULL;
		m_head->parent = NULL;
		m_head = NULL;
		return true;
	}
	else if (m_head->left == NULL && m_head->right != NULL ||
		m_head->left != NULL && m_head->right == NULL) {
		if (parent->right == m_head) {
			parent->right = m_head->left;
			m_head->parent = parent;
		}
		else if (parent->left == m_head) {
			parent->left = m_head->right;
			m_head->parent = parent;
		}
		m_head = NULL;
		return true;
	}
	else {

	}

	m_head = NULL;
	return true;
}