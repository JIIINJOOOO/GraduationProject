#include "Boss.h"
#include <map>
#include <array>
#include <random>
#include <chrono>
#include "CPlayer.h"
#include "CMonster.h"
#include "Event.h"

extern map<int, CPlayer*> g_player;

default_random_engine dre;
uniform_int_distribution<> uid;

void Boss::Initialize(const int& id, const Position& p) {
	HealthPoint& hp = healthPoint;
	hp.body_hp = MAX_BODY_HP;
	hp.left_hand_hp = hp.right_hand_hp = MAX_HAND_HP;
	hp.left_leg_hp = hp.right_leg_hp = MAX_LEG_HP;
	hp.head_hp = MAX_HEAD_HP;

	isDestroy.body = false;
	isDestroy.head = false;
	isDestroy.leftHand = false;
	isDestroy.rightHand = false;
	isDestroy.leftLeg = false;
	isDestroy.rightLeg = false;

	defaultPos = pos = p;
	target = -1;
	this->id = id;
	state = BossIdle::GetInstance();
	isActive = false;

	pathFinder = new CPathFinder;
	// AddTimer(0, 4, high_resolution_clock::now(), 0);
}

void Boss::Respawn() {
	HealthPoint& hp = healthPoint;
	hp.body_hp = MAX_BODY_HP;
	hp.left_hand_hp = hp.right_hand_hp = MAX_HAND_HP;
	hp.left_leg_hp = hp.right_leg_hp = MAX_LEG_HP;
	hp.head_hp = MAX_HEAD_HP;

	isDestroy.body = false;
	isDestroy.head = false;
	isDestroy.leftHand = false;
	isDestroy.rightHand = false;
	isDestroy.leftLeg = false;
	isDestroy.rightLeg = false;

	pos = defaultPos;
	target = -1;
	isActive = false;
	chasePath.clear();
}

B_STATE Boss::Idle() {
	// return B_DEAD;

	// 타겟 플레이어 검색
	array<int, MAX_PLAYER> disArray;
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		int dis = GetDistance(g_player[i]->GetPosition(), pos);
		if (dis < CHASE_RANGE) disArray[i] = dis;
	}

	int chaseID = 0;
	if (target == NO_DETECTED)
		for (int i = 0; i < MAX_PLAYER; ++i)
			if (disArray[i] != NO_DETECTED)
				if (disArray[i] < disArray[chaseID])
					chaseID = i;
	if (disArray[chaseID] == NO_DETECTED) chaseID = NO_DETECTED;
	
	// 1
	if (isDestroy.rightLeg && isDestroy.rightLeg)
		return B_THROW_ATK;
	
	if (50 < disArray[target] && disArray[target] < 200) // 돌진
		if (!isDestroy.rightLeg && !isDestroy.leftLeg) 
			return B_DASH_ATK;

	if (disArray[target] <= 50) {
	// 손 휘두르기 or 30퍼 확률로 바닥찍기
		if (uid(dre) % 3 < 2) return B_SWING_ATK;
		else return B_STRIKE_ATK;
	}
	else return B_CHASE;
	return B_IDLE;
}

B_STATE Boss::Chase() {
	if (target == NO_DETECTED) return B_IDLE;
	// if (is_targetMove) chasePath.clear();
	auto& target_p = g_player[target];
	if (chasePath.size() == 0) {
		POS_2D my = { (pos.x - SIDE_MIN) / POS_SCALE, (pos.y - SIDE_MIN) / POS_SCALE };
		Position pp = target_p->GetPosition();
		POS_2D other = { (pp.x - SIDE_MIN) / POS_SCALE, (pp.y - SIDE_MIN) / POS_SCALE };
		chasePath = pathFinder->GetPath(my, other);
	}
	POS_2D p = chasePath.front();
	// pos = p;
	// 임시로 좌표에 그대로 넣었는데 노드의 좌표이므로 사이 보정 해줘야함
	// 이걸 어케함 씨;발ㅋㅋㅋㅋㅋㅋㅋㅋ 조졌네ㅋㅋ
	pos.x = (p.x*POS_SCALE) + SIDE_MIN;
	pos.y = (p.y*POS_SCALE) + SIDE_MIN;
	// cout << "pos : " << pos.x << "\t" << pos.y << endl;
	chasePath.pop_front();

	if (chasePath.size() == 0) return B_IDLE;	// 끝까지 이동했다는 뜻이니까 idle 상태로 넘어가 다음 행동을 찾는다.
	else if (GetDistance(pos, target_p->GetPosition()) < ATTACK_RANGE) {
		chasePath.clear();
		return B_ATTACK;	// 공격 범위에 들어가면 공격으로 전환하고 AttackState에서 어떤 공격할지 결정
	}
	else if (GetDistance(pos, target_p->GetPosition()) > ACTIVITY_RANGE){
		// 재탐색 후 주변에 아무도 없으면 AI 비활성화
		chasePath.clear();
		target = NO_DETECTED;
		return B_IDLE;
	}
	return B_CHASE;
}

B_STATE Boss::Attack() {
	// 다리 파괴시 던지기만 사용한다
	if (isDestroy.leftLeg || isDestroy.rightLeg)
		if (!(isDestroy.rightHand && isDestroy.leftHand))
			return B_THROW_ATK;
		else return B_IDLE;
	// 손이 남아있으면 휘두르기 or 내려치기 -> 어느 손으로 할지는 저 안에서 결정하기
	if (!(isDestroy.rightHand && isDestroy.leftHand)) {
		if (isDestroy.head) {}	// 머가리 깨졌으면 아무 행동안하고 밑으로 내려버리기
		else if (GetDistance(pos, g_player[target]->GetPosition()) > 200)
			return B_DASH_ATK;
		else if (uid(dre) % 3 < 2) return B_SWING_ATK;
		return B_STRIKE_ATK;
	}
	// 머가리깨지면 랜덤
	if (isDestroy.head) {
		if (isDestroy.leftHand && isDestroy.rightHand)
			return B_DASH_ATK;
		switch (uid(dre) % 10) {
		case 0: return B_SWING_ATK;	break;
		case 1: return B_SWING_ATK;	break;
		case 2: return B_THROW_ATK;	break;
		case 3: return B_STRIKE_ATK;break;
		case 4: return B_DASH_ATK;	break;
		default: return B_ATTACK;	break;	// 항상 공격패턴이 나오는 것도 이상하니까 일정 확률로 한번 넘기자
		}
	}
	return B_ATTACK;
}

void Boss::Update() {
	if (GetTotalHP() <= 0) ChangeState(BossDead::GetInstance());
	else IsPartDestroyed();
	state->Execute(this);
	auto nextTime = chrono::high_resolution_clock::now() + 1s;
	// 일단 임의로 1초로 설정 추후 애니메이션 프레임도 보고 실제 돌아가는거 보고 상태별로 조정필요함
	if (state == BossDead::GetInstance()) nextTime += 60s;	// 스폰쿨타임
	bool keepAlive = false;
	for (int i = 0; i < MAX_PLAYER; ++i)
		if (g_player[i] != NULL)
			if (GetDistance(g_player[i]->GetPosition(), pos) < MAX_VIEW_RANGE) {
				keepAlive = true;
				break;
			}
	if (keepAlive) 
		AddTimer(id, 4, nextTime, target);	// EV_BOSS
	else isActive = false;
}

void Boss::IsPartDestroyed() {  
	if (healthPoint.left_hand_hp <= 0) isDestroy.leftHand = true;
	if (healthPoint.right_hand_hp <= 0) isDestroy.rightHand = true;
	if (healthPoint.left_leg_hp <= 0) isDestroy.leftLeg = true;
	if (healthPoint.right_leg_hp <= 0) isDestroy.rightLeg = true;
	if (healthPoint.head_hp <= 0) isDestroy.head = true;
	if (healthPoint.body_hp <= 0) isDestroy.body = true;
}

void Boss::ChangeState(BossState* ns){
	state->Exit(this);
	state = ns;
	state->Enter(this);
}

IsDestroy Boss::GetIsDestroy() const {
	return isDestroy;
}

Position Boss::GetPosition() const {
	return pos;
}

int Boss::GetTotalHP() const {
	return healthPoint.body_hp + healthPoint.head_hp + healthPoint.left_hand_hp +
		healthPoint.right_hand_hp + healthPoint.left_leg_hp + healthPoint.right_leg_hp;;
}

void Boss::SetPosition(const Position& p) {
	pos = p;
}

void Boss::SetRotation(const Position& p) {
	rotation = p;
}

SC_OBJECT_ENTER Boss::MakeEnterPacket() {
	SC_OBJECT_ENTER p;
	p.size = sizeof(SC_OBJECT_ENTER);
	p.type = sc_enter_obj;
	p.o_type = 0;	/// 나중에 이걸로 몬스터 종류 나누고 그래야함
	p.oid = id;
	p.pos = pos;
	return p;
}

SC_UPDATE_OBJ Boss::MakeUpdatePacket() {
	SC_UPDATE_OBJ p;
	p.type = sc_update_obj;
	p.size = sizeof(SC_UPDATE_OBJ);
	p.oid = 0;
	p.pos = pos;

	return p;
}