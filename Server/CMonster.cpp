#include "CMonster.h"
#include "globals.h"
#include "CServer.h"
using namespace std;
extern short board[SIDE_LEN][SIDE_LEN];
void CMonster::Update(CObject& my, const CObject& other) {
	Position myPos = my.GetPosition();
	Position otherPos = other.GetPosition();
	int distance = my.GetDistance(otherPos);
	// cout << myPos.x << "\t" << myPos.y << "\tsta: " << m_state << endl;
	if (m_state == chase) {
		// 여기서 추적해야하잖아 
		// x, y축으로 이동하고 z축은 일단 신경쓰지 않는다
		if (distance < ATTACK_RANGE) { 
			m_state = attack;
			return;
		}
		if (my.GetDistance(m_defaultPos) > CHASE_RANGE) {
			m_state = return_home;
			return;
		}
		
		// m_chasePath = m_pathFinder->GetPath(myPos, otherPos);
		// for (int i = 0; i < m_chasePath.size(); ++i) {
		// 	// myPos = m_chasePath.pop_front();
		// 	// m_chasePath.
		// 	myPos = m_chasePath.front();
		// 	m_chasePath.pop_front();
		// 	Sleep(1000 / 400);
		// 	// printf("%d  %d  %d \n", myPos.x, myPos.y, myPos.z);
		// }
		// m_chasePath.clear();

		float x_dis = myPos.x - otherPos.x;
		float y_dis = myPos.y - otherPos.y;
		x_dis=myPos.x + (-x_dis / sqrt((x_dis*x_dis) + (y_dis*y_dis)))*3.f;
		y_dis=myPos.y + (-y_dis / sqrt((x_dis*x_dis) + (y_dis*y_dis)))*3.f;
		
		float desZ = board[(int)x_dis + X_SIDE][(int)y_dis + Y_SIDE];
		if (desZ > (myPos.z + 20.f))
			return;
		myPos.x = x_dis;
		myPos.y = y_dis;
		myPos.z = desZ;
	}
	else if (m_state == return_home) {
		if (distance < ATTACK_RANGE) {
			m_state = attack;
			return;
		}
		if (my.GetDistance(m_defaultPos) < IDLE_RANGE) {
			m_state = idle;
			return;
		}
		float x_dis = myPos.x - m_defaultPos.x;
		float y_dis = myPos.y - m_defaultPos.y;
		x_dis = myPos.x + (-x_dis / sqrt((x_dis*x_dis) + (y_dis*y_dis)))*3.f;
		y_dis = myPos.y + (-y_dis / sqrt((x_dis*x_dis) + (y_dis*y_dis)))*3.f;
		float desZ = board[(int)x_dis + X_SIDE][(int)y_dis + Y_SIDE];
		cout << desZ << endl;
		if (desZ > (myPos.z + 20.f))
			return;
		myPos.x = x_dis;
		myPos.y = y_dis;
		myPos.z = desZ;
	}
	if (m_state == attack) {
	}
	my.SetPosition(myPos);
}

void CMonster::Initialize(Position pos, int type) {
	SetDefaultPos(pos);
	SetActivityRange(ACTIVITY_RANGE);
	m_healthPoint = MONSTER_MAX_HP;
	m_type = type;
	xIdx = pos.x;
	zIdx = pos.z;
	m_pathFinder = new CPathFinder;
}

// Getter & Setter
void CMonster::SetState(const int& state) {
	m_state = state;
}
void CMonster::SetDefaultPos(const Position& pos) {
	m_defaultPos = pos;
}
void CMonster::SetActivityRange(const int& range) {
	m_activityRange = range;
}
int CMonster::GetHealthPoint() const {
	return m_healthPoint;
}
int CMonster::GetState() const {
	return m_state;
}

void CMonster::SetChaseObj(const CObject& other) {
	// if (m_state == chase)
}