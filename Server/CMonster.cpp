#include "CMonster.h"
#include "CLobby.h"

void CMonster::Update(const CObject& other) {
	/*
	1. �ڽ��� �þ߿� �÷��̾ �ִ��� �˻� �ִٸ� ����
	2. ���� ������ ���Դٸ� ����
	3. �ִ� ���� ������ ����ٸ� ���� ��ġ�� ���ƿ�
	*/
	Position pos;

	int distance;
	for (int i = 0; i < MAX_PLAYER; ++i) {
		
		// if (users[i] == None) continue;
		// distance = g_players[users[i]]->GetDistance(m_obj->GetPosition());
		if (distance < ACTIVITY_RANGE) {
			/*
			���� ������ ������ �÷��̾ �������Ƿ� ������ �ϱ� �ϴµ� 
			1. ������ �þ� ����ؾ��� ������� ������ �޸��� ���ݾ�?
			2. ���߿� �ͷ����̵� ���� ��������, �װɷ� �þ߿� ������ ������ Ž���غ��� �͵� ������
			*/
			m_state = chase;
		//	Chase(*g_players[i]);
			if (distance < ATTACK_RANGE) {
				// ���� ������ ������ ����
				m_state = attack;
			}
		}
	}
	// ������ ��ġ���� ���� ������ ����°�
	// int distanceToActivityRange = m_obj->GetDistance(m_defaultPos);
	// if (distanceToActivityRange > ACTIVITY_RANGE) {
	// 	// ������ �ڸ� (m_difaultPos)�� ���ư���
	// }
}

void CMonster::Chase(const CPlayer& target) {
}
void CMonster::Initialize(Position pos, int type) {
	SetDefaultPos(pos);
	SetActivityRange(ACTIVITY_RANGE);
	m_healthPoint = MONSTER_MAX_HP;
	m_type = type;
	xIdx = pos.x;
	zIdx = pos.z;
}

// Getter & Setter
void CMonster::SetObjIdx(const int& idx) {
	// m_objIdx = idx;
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
Position CMonster::GetPosition() const {
//	return m_obj->GetPosition();
}