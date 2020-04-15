#include "node.h"

bool Overlap_Start_End(Position s, Position e) {
	if (s == e) return true;
	return false;
}

float GetDistance(Position p, Position e) {
	float x = pow((p.x - e.x), 2);
	float y = pow((p.y - e.y), 2);
	float z = pow((p.z - e.z), 2);
	float distance = sqrt(x + y + z);
	return abs(distance);
}

Node::Node(Position pos, Node* parent, int g, Position h) {
	currentPos = pos;
	this->parent = parent;
	costG = g;
	SetCostH(h);
}

int Node::GetCostG() const {
	return costG;
}

float Node::GetCostH() const {
	return costH;
}

int Node::GetCostF() const {
	return ((float)costG + costH);
}

Node* Node::GetParent() const {
	return parent;
}

Position Node::GetPosition() const {
	return currentPos;
}

void Node::SetCostG(const int& g) {
	costG = g;
}

void Node::SetCostH(Position p) {
	costH = GetDistance(currentPos, p);
}