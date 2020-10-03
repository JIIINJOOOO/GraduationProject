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

extern void send_packet(int uid, void* p);

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

	BoneMap[R_HAND] = 20;
	BoneMap[R_FOREARM] = 22;
	BoneMap[R_UPPERARM] = 24;
	BoneMap[R_FOOR] = 20;
	BoneMap[R_CALF] = 22;
	BoneMap[R_THIGHT] = 24;
	BoneMap[L_HAND] = 5;
	BoneMap[L_FOREARM] = 7;
	BoneMap[L_UPPERARM] = 9;
	BoneMap[L_FOOR] = 20;
	BoneMap[L_CALF] = 22;
	BoneMap[L_THIGHT] = 24;
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

	// 展為 巴傾戚嬢 伊事
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
	
	if (50 < disArray[target] && disArray[target] < 200) // 宜遭
		if (!isDestroy.rightLeg && !isDestroy.leftLeg) 
			return B_DASH_ATK;

	if (disArray[target] <= 50) {
	// 謝 番砧牽奄 or 30遁 溌懸稽 郊韓啄奄
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
	// 績獣稽 疎妊拭 益企稽 隔醸澗汽 葛球税 疎妊戚糠稽 紫戚 左舛 背操醤敗
	// 戚杏 嬢追敗 松;降せせせせせせせせ 繕然革せせ
	pos.x = (p.x*POS_SCALE) + SIDE_MIN;
	pos.y = (p.y*POS_SCALE) + SIDE_MIN;
	// cout << "pos : " << pos.x << "\t" << pos.y << endl;
	chasePath.pop_front();

	if (chasePath.size() == 0) return B_IDLE;	// 魁猿走 戚疑梅陥澗 倶戚艦猿 idle 雌殿稽 角嬢亜 陥製 楳疑聖 達澗陥.
	else if (GetDistance(pos, target_p->GetPosition()) < ATTACK_RANGE) {
		chasePath.clear();
		return B_ATTACK;	// 因維 骨是拭 級嬢亜檎 因維生稽 穿発馬壱 AttackState拭辞 嬢恐 因維拝走 衣舛
	}
	else if (GetDistance(pos, target_p->GetPosition()) > ACTIVITY_RANGE){
		// 仙貼事 板 爽痕拭 焼巷亀 蒸生檎 AI 搾醗失鉢
		chasePath.clear();
		target = NO_DETECTED;
		return B_IDLE;
	}
	return B_CHASE;
}

B_STATE Boss::Attack() {
	// 陥軒 督雨獣 揮走奄幻 紫遂廃陥
	if (isDestroy.leftLeg || isDestroy.rightLeg)
		if (!(isDestroy.rightHand && isDestroy.leftHand))
			return B_THROW_ATK;
		else return B_IDLE;
	// 謝戚 害焼赤生檎 番砧牽奄 or 鎧形帖奄 -> 嬢汗 謝生稽 拝走澗 煽 照拭辞 衣舛馬奄
	if (!(isDestroy.rightHand && isDestroy.leftHand)) {
		if (isDestroy.head) {}	// 袴亜軒 凹然生檎 焼巷 楳疑照馬壱 購生稽 鎧形獄軒奄
		else if (GetDistance(pos, g_player[target]->GetPosition()) > 200)
			return B_DASH_ATK;
		else if (uid(dre) % 3 < 2) return B_SWING_ATK;
		return B_STRIKE_ATK;
	}
	// 袴亜軒凹走檎 沓棋
	if (isDestroy.head) {
		if (isDestroy.leftHand && isDestroy.rightHand)
			return B_DASH_ATK;
		switch (uid(dre) % 10) {
		case 0: return B_SWING_ATK;	break;
		case 1: return B_SWING_ATK;	break;
		case 2: return B_THROW_ATK;	break;
		case 3: return B_STRIKE_ATK;break;
		case 4: return B_DASH_ATK;	break;
		default: return B_ATTACK;	break;	// 牌雌 因維鳶渡戚 蟹神澗 依亀 戚雌馬艦猿 析舛 溌懸稽 廃腰 角奄切
		}
	}
	return B_ATTACK;
}

void Boss::Update() {
	if (GetTotalHP() <= 0) ChangeState(BossDead::GetInstance());
	else IsPartDestroyed();
	state->Execute(this);
	auto nextTime = chrono::high_resolution_clock::now() + 1s;
	// 析舘 績税稽 1段稽 竺舛 蓄板 蕉艦五戚芝 覗傾績亀 左壱 叔薦 宜焼亜澗暗 左壱 雌殿紺稽 繕舛琶推敗
	if (state == BossDead::GetInstance()) nextTime += 60s;	// 什肉悌展績
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

void Boss::BoneMapUpdate(char* new_boneMap) {
	for (int i = 0; i < NUM_PARTS; ++i) {
		boneLock.lock();
		if (new_boneMap[i] < BoneMap[i]) {
			if (BoneMap[i] <= 0) {
				boneLock.unlock();
				continue;
			}
			BoneMap[i] -= 1;
			boneLock.unlock();
			if (BoneMap[i] <= 0) {
				cout << "左什 採是 督雨" << endl;
				SC_BONE_BREAK pack{ sizeof(SC_BONE_BREAK), sc_bone_break, id, i };
				for (int i = 0; i < MAX_PLAYER; ++i) {
					if (g_player[i] == NULL) continue;
					send_packet(i, &pack);
				}
			}
			else {
				SC_BONE_UPDATE pack{ sizeof(SC_BONE_UPDATE), sc_bone_update, id, i };
				pack.attacked = BoneMap[i];
				for (int i = 0; i < MAX_PLAYER; ++i) {
					if (g_player[i] == NULL) continue;
					send_packet(i, &pack);
				}
			}
		}
		else boneLock.unlock();
	}
	
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
	p.o_type = OBJ_GOLEM;	/// 蟹掻拭 戚杏稽 佼什斗 曽嫌 蟹刊壱 益掘醤敗
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