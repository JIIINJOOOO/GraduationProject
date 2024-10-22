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
	hp.right_calf_hp = hp.right_calf_hp = MAX_CALF_HP;
	hp.left_forearm_hp = hp.right_forearm_hp = MAX_FOREARM_HP;
	hp.left_calf_hp = hp.right_calf_hp = MAX_CALF_HP;
	hp.left_foot_hp = hp.right_foot_hp = MAX_FOOT_HP;
	hp.left_thigh_hp = hp.right_thigh_hp = MAX_THIGH_HP;
	hp.head_hp = MAX_HEAD_HP;

	isDestroy.head = false;
	isDestroy.leftHand = false;
	isDestroy.rightHand = false;
	isDestroy.rightForearm = false;
	isDestroy.rightUpperarm = false;
	isDestroy.rightFoot = false;
	isDestroy.rightThigh = false;
	isDestroy.rightCalf = false;
	isDestroy.leftForearm = false;
	isDestroy.leftUpperarm = false;
	isDestroy.leftFoot = false;
	isDestroy.leftThigh = false;
	isDestroy.leftCalf = false;

	defaultPos = pos = p;
	target = -1;
	this->id = id;
	state = BossIdle::GetInstance();
	isActive = true;
	
	pathFinder = new CPathFinder;
	// AddTimer(0, 4, high_resolution_clock::now(), 0);
	pos = { 22976.5,35731.7,-932.3 };

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

	mainHP = 600;
}

B_STATE Boss::Idle() {
	// return B_DEAD;
	// cout << "Boss idle\n";
	// auto dis = BOSS_ACTIVITY_RANGE;
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		auto p_dis = GetDistance(pos, g_player[i]->GetPosition());
		if (p_dis < BOSS_ACTIVITY_RANGE && p_dis > 0) return B_ATTACK;
	}
	return B_IDLE;

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
	if (isDestroy.rightFoot && isDestroy.rightFoot)
		return B_THROW_ATK;
	
	if (50 < disArray[target] && disArray[target] < 200) // 宜遭
		if (!isDestroy.rightFoot && !isDestroy.leftFoot) 
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
	// if (isDestroy.leftFoot || isDestroy.rightFoot)
	// 	if (!(isDestroy.rightHand && isDestroy.leftHand))
	// 		return B_THROW_ATK;
	// 	else return B_IDLE;

	int targetID = 0;
	int targetDis = BOSS_ACTIVITY_RANGE;
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		auto dis = GetDistance(pos, g_player[i]->GetPosition());
		if (targetDis < dis) {
			if (dis < 0) continue;
			if (dis > BOSS_ACTIVITY_RANGE) continue;
			targetID = i;
			targetDis = dis;
		}
	}
	if (g_player[targetID] != NULL) target = targetID;
	if (targetDis < 0 || targetDis > BOSS_ACTIVITY_RANGE) return B_ATTACK;

	if (IsLegBroken()) {
		return B_DOWN_ATK;
		// if (!isDestroy.leftHand && !isDestroy.rightHand) {
		// 	// switch (uid(dre) % 5) {
		// 	// case 0: return B_HANDCLAP_ATK; break;
		// 	// case 1: return B_SWING_ATK; break;
		// 	// case 2: return B_PUNCH_ATK; break;
		// 	// case 3: return B_DOWN_ATK; break;
		// 	// case 4: return B_NORMAL_ATK;  break;
		// 	// }
		// 	return B_DOWN_ATK;
		// }
		// else if (isDestroy.leftHand) {
		// 	if (isDestroy.rightHand) return B_NORMAL_ATK;
		// 	return B_SWING_ATK;
		// }
		// else if (isDestroy.rightHand) {
		// 	if (isDestroy.leftHand) return B_NORMAL_ATK;
		// 	switch (uid(dre) % 2){
		// 	case 0: return B_PUNCH_ATK; break;
		// 	case 1: return B_NORMAL_ATK; break;
		// 	}
		// }
		/*
		図 葛菰 呑帖
		神 什成
		*/

	}
	else {
		// 但揮走奄, 宜揮走奄, 宜遭, 高奄
		if (g_player[target] == NULL) return B_IDLE;
		targetDis = GetDistance(pos, g_player[target]->GetPosition());
		if (targetDis > B_LONG_ATK_RANGE) {
			switch (uid(dre) % 3) {
			case 0: return B_THROW_ATK; break;
			case 1: return B_DASH_ATK; break;
			case 2: return B_ICESPEAR_ATK; break;
			}
		}
		else {
			switch (uid(dre) % 5) {
			case 0: return B_STOMP_ATK; break;
			case 1: return B_HANDCLAP_ATK; break;
			case 2: return B_SWING_ATK; break;
			case 3: return B_NORMAL_ATK; break;
			case 4: return B_PUNCH_ATK; break;
			}
		}
	}
	return B_ATTACK;

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
	if (healthPoint.body_hp <= 0) {
		ChangeState(BossDead::GetInstance());
		// AddTimer(id, 4, chrono::high_resolution_clock::now()+1s, target);
		return;
	}
	if (mainHP <= 0) {
		ChangeState(BossDead::GetInstance());
		// AddTimer(id, 4, chrono::high_resolution_clock::now() + 1s, target);
		return;
	}
	else IsPartDestroyed();
	state->Execute(this);
	// auto nextTime = chrono::high_resolution_clock::now() + 1s;
	
	// 析舘 績税稽 1段稽 竺舛 蓄板 蕉艦五戚芝 覗傾績亀 左壱 叔薦 宜焼亜澗暗 左壱 雌殿紺稽 繕舛琶推敗
	// if (state == BossDead::GetInstance()) nextTime += 60s;	// 什肉悌展績
	// bool keepAlive = false;
	// for (int i = 0; i < MAX_PLAYER; ++i)
	// 	if (g_player[i] != NULL)
	// 		if (GetDistance(g_player[i]->GetPosition(), pos) < MAX_VIEW_RANGE) {
	// 			keepAlive = true;
	// 			break;
	// 		}
	// if (keepAlive) 
	// AddTimer(id, 4, nextTime, target);	// EV_BOSS
	// else isActive = false;
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
				// cout << "左什 採是 督雨" << endl;
				BoneBreak(i);
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

void Boss::BoneBreak(const int& part) {
	switch (part) {
	case R_HAND:
		cout << "Boss's R_HAND is Break\n";
		healthPoint.right_hand_hp = 0;
		isDestroy.rightHand = true;
		break;
	case R_FOREARM:
		cout << "Boss's R_FOREARM is Break\n";
		healthPoint.right_forearm_hp = 0;
		isDestroy.rightForearm = true;
		break;
	case R_UPPERARM:
		cout << "Boss's R_UPPERARM is Break\n";
		healthPoint.right_upperarm_hp = 0;
		isDestroy.rightUpperarm = true;
		break;
	case R_FOOR:
		cout << "Boss's R_FOOT is Break\n";
		healthPoint.right_foot_hp = 0;
		isDestroy.rightFoot = true;
		break;
	case R_CALF:
		cout << "Boss's R_CALF is Break\n";
		healthPoint.right_calf_hp = 0;
		isDestroy.rightCalf = true;
		break;
	case R_THIGHT:
		cout << "Boss's R_THIGHT is Break\n";
		healthPoint.right_thigh_hp = 0;
		isDestroy.rightThigh = true;
		break;
	case L_HAND:
		cout << "Boss's L_HAND is Break\n";
		healthPoint.left_hand_hp = 0;
		isDestroy.leftHand = true;
		break;
	case L_FOREARM:
		cout << "Boss's L_FOREARM is Break\n";
		healthPoint.left_forearm_hp = 0;
		isDestroy.leftForearm = true;
		break;
	case L_UPPERARM:
		cout << "Boss's L_UPPERARM is Break\n"; 
		healthPoint.left_upperarm_hp = 0;
		isDestroy.leftUpperarm = true;
		break;
	case L_FOOR:
		cout << "Boss's L_FOOT is Break\n"; 
		healthPoint.left_foot_hp = 0;
		isDestroy.leftFoot = true;
		break;
	case L_CALF:
		cout << "Boss's L_CALF is Break\n"; 
		healthPoint.left_calf_hp = 0;
		isDestroy.leftCalf = true;
		break;
	case L_THIGHT:
		cout << "Boss's L_THIGHT is Break\n"; 
		healthPoint.left_thigh_hp = 0;
		isDestroy.leftThigh = true;
		break;
	}
}

void Boss::IsPartDestroyed() {  
	if (healthPoint.left_hand_hp <= 0) isDestroy.leftHand = true;
	if (healthPoint.right_hand_hp <= 0) isDestroy.rightHand = true;
	if (healthPoint.left_forearm_hp <= 0) isDestroy.leftForearm = true;
	if (healthPoint.right_forearm_hp <= 0) isDestroy.rightForearm = true;
	if (healthPoint.head_hp <= 0) isDestroy.head = true;
}

void Boss::TakeDamage(const int& damage) {
	mainHP -= damage;
	if (mainHP <= 0) {
		// Death
		mainHP = 0;
		SC_DEAD pack{ sizeof(SC_DEAD), sc_dead, id };
		for (int i = 0; i < MAX_PLAYER; ++i) {
			if (g_player[i] == NULL) continue;
			send_packet(i, &pack);
		}
	}
	else {
		SC_DAMAGED pack{ sizeof(SC_DAMAGED), sc_damaged, id, mainHP };
		for (int i = 0; i < MAX_PLAYER; ++i) {
			if (g_player[i] == NULL) continue;
			send_packet(i, &pack);
		}
	}
}


bool Boss::IsLegBroken() const{
	if (isDestroy.rightFoot) return true;
	if (isDestroy.rightThigh) return true;
	if (isDestroy.rightCalf) return true;
	if (isDestroy.leftFoot) return true;
	if (isDestroy.leftThigh) return true;
	if (isDestroy.leftCalf) return true;
	return false;
}

bool Boss::IsRHandBroken() const {
	if (isDestroy.rightForearm) return true;
	if (isDestroy.rightUpperarm) return true;
	if (isDestroy.rightHand) return true;
	return false;
}

bool Boss::IsLHandBroken() const {
	if (isDestroy.leftForearm) return true;
	if (isDestroy.leftUpperarm) return true;
	if (isDestroy.leftHand) return true;
	return false;
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
	
}

void Boss::SetPosition(const Position& p) {
	pos = p;
}

void Boss::SetRotation(const Position& p) {
	rotation = p;
}

int Boss::GetTarget() const {
	return target;
}

int Boss::GetID() const {
	return id;
}

int Boss::GetMainHP() const {
	return mainHP;
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