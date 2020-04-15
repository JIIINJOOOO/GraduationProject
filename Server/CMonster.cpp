#include "CMonster.h"
#include "CLobby.h"

void CMonster::Update(const CObject& other) {
	/*
	1. 자신의 시야에 플레이어가 있는지 검사 있다면 추적
	2. 공격 범위에 들어왔다면 공격
	3. 최대 추적 범위를 벗어났다면 원래 위치로 돌아옴
	*/
	Position pos;

	int distance;
	for (int i = 0; i < MAX_PLAYER; ++i) {
		
		// if (users[i] == None) continue;
		// distance = g_players[users[i]]->GetDistance(m_obj->GetPosition());
		if (distance < ACTIVITY_RANGE) {
			/*
			추적 가능한 범위에 플레이어가 들어왔으므로 추적을 하긴 하는데 
			1. 몬스터의 시야 고려해야함 뒤통수에 눈깔이 달리진 않잖아?
			2. 나중에 터레인이든 뭐든 가져오면, 그걸로 시야에 들어오는 영역만 탐지해보는 것도 좋을듯
			*/
			m_state = chase;
		//	Chase(*g_players[i]);
			if (distance < ATTACK_RANGE) {
				// 공격 가능한 범위에 들어옴
				m_state = attack;
			}
		}
	}
	// 원래의 위치에서 추적 범위를 벗어났는가
	// int distanceToActivityRange = m_obj->GetDistance(m_defaultPos);
	// if (distanceToActivityRange > ACTIVITY_RANGE) {
	// 	// 원래의 자리 (m_difaultPos)로 돌아간다
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