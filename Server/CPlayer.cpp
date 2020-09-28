#include "CPlayer.h"
#include "CMonster.h"
#include "Boss.h"
#include "protocol.h"
#include "Event.h"
#include <map>
#include "CDBConnector.h"


extern map<int, CPlayer*> g_player;
extern map<int, CMonster*> g_monster;
extern Boss *boss;

extern void send_packet(int uid, void* p);

#define EV_MONSTER 3
#define EV_UPDATE 7
#define EV_BERSER 8
#define EV_ATK_OFF 9
#define EV_GUARD_OFF 10
#define EV_EVADE_OFF 11
#define EV_FIREBALL 12
#define EV_ASS_OFF 13

CPlayer::CPlayer(std::string id, std::string pass) {
	name = id; 
	m_pass = pass;
}
CPlayer::~CPlayer() {
	name.clear();
	m_pass.clear();
}
void CPlayer::Initialize(const CPlayer& p) {
	name = p.name;
	m_pass = p.m_pass;
	level = p.level;
	pos = { 14463.1f,78765.1f, -444.2f };
	/*
	28600.0
	77420.0
	-39726.15625
	*/
}

void CPlayer::Initialize(int hp, int o_type, int exp, int lv, int mp, int atk, int x, int y){
	MaxHP = healthPoint = hp;
	MaxMP = magicPoint = mp;
	objType = o_type;
	this->exp = exp;
	level = lv;
	atkPoint = atk;
	atkPoint = 15;
	pos.x = x;
	pos.y = y;
	pos = { 10230.0, 76530.0, -360 };
	isBerserk = false;
	isGuard = false;
	isWpnOn = false;
	isAlive = true;
	isEvade = false;
	isSpellFireball = false;
	isHide = false;
	isAssasinate = false;
	comboCnt = 0;
	lastAtkTime = high_resolution_clock::now();	//
	rotation = { 0,0,0 };
	wpnType = wpn_none;
}

bool CPlayer::operator== (const CPlayer& cp) const {
	return name == cp.name;
}

void CPlayer::SetState(const int& state) {
	this->state = state;
}
int CPlayer::GetState() {
	return state;
}
std::string CPlayer::GetID() {
	return name;
}

void CPlayer::Ready() {
	// state = ready;
	if (isReady) isReady = false;
	else isReady = true;
}
void CPlayer::Attck() {
	if (isAttack) return;
	if (wpnType == wpn_none) return;
	isAttack = true;

	// 마지막 공격으로부터 1초 이내에 공격시 콤보 증가
	if (lastAtkTime + 1s > high_resolution_clock::now())
		comboCnt = (comboCnt + 1) % MAX_SWORD_COMBO;
	else comboCnt = 0;	// reset

	SC_OBJ_ATTACK pack{ sizeof(SC_OBJ_ATTACK), sc_attack, id, comboCnt };
	for (auto& p : viewList)
		if (p < MAX_PLAYER) send_packet(p, &pack);
	// send_packet(id, &pack);
	// Sleep(500);
	for (auto& oid : viewList) {
		if (oid < NPC_ID_START) continue;
		cout << oid<<"\t"<<GetDistance(g_monster[oid]->GetPosition()) << endl;
		if (GetDistance(g_monster[oid]->GetPosition()) < ATTACK_RANGE) {
			Position directionVec = { cos(rotation.y), sin(rotation.y), 0 };
			
			cout << name << "의 공격으로 몬스터" << oid << "에게 " << atkPoint << "의 대미지!" << endl;
			if (g_monster[oid]->TakeDamage(atkPoint) <= 0)
				KillMonster(oid);
		//	break;
		}
	}
	AddTimer(id, EV_ATK_OFF, high_resolution_clock::now() + 500ms, NULL);	// 공격 종료를 알림
}

void CPlayer::Berserk() {
	if (isBerserk) return;
	if (wpnType == wpn_none) return;
	cout << name << "(이)가 버서크 스킬을 사용" << endl;
	
	SC_BERSERK pack{sizeof(SC_BERSERK), sc_berserk, id};
	for (auto& p : viewList)
		if (p < MAX_PLAYER) send_packet(p, &pack);
	// Sleep(50);
	for (auto& oid : viewList) {
		if (oid < NPC_ID_START) continue;
		if (GetDistance(g_monster[oid]->GetPosition()) < BERSERK_RANGE) {
			if (g_monster[oid]->TakeDamage(200) <= 0)
				KillMonster(oid);
			cout << name << "의 버서크로 몬스터" << oid << "에게 " << 200 << "의 대미지!" << endl;
		}
	}
	isBerserk = true;
	atkPoint += BERSERK_BONUS;
	magicPoint += BERSERK_BONUS;
	AddTimer(id, EV_BERSER, high_resolution_clock::now()+10s, 0); // 시간은 추후 지정
}

void CPlayer::BerserkOff() {
	isBerserk = false;
	atkPoint -= BERSERK_BONUS;
	magicPoint -= BERSERK_BONUS;
	cout << name << "의 버서크 시간이 종료" << endl;
}

void CPlayer::KillMonster(int oid) {
	cout << name << "(이)가 몬스터" << oid << "를 죽이고 경험치 " << g_monster[oid]->GetEXP() << "획득" << endl;
	exp += g_monster[oid]->GetEXP();
	send_packet(id, &MakeGetExpPacket());

	SC_DEAD pack{ sizeof(SC_DEAD), sc_dead, oid };
	send_packet(id, &pack);
	for (auto& p : viewList)
		if (p < MAX_PLAYER)
			send_packet(p, &pack);
	if (exp >= 100) LevelUp();
}

void CPlayer::LevelUp() {
	++level;
	cout << name << " 레벨업(" << level << ")\n";
	exp -= 100;
	atkPoint += LEVEL_UP_BONUS;
	MaxHP += LEVEL_UP_BONUS;
	MaxMP += LEVEL_UP_BONUS;
	healthPoint = MaxHP;
	magicPoint  = MaxMP;
	SC_LEVEL_UP pack{ sizeof(SC_LEVEL_UP), sc_level_up , id, level, exp, healthPoint, magicPoint };
	send_packet(id, &pack);
	for (auto& p : viewList)
		if (p <= MAX_PLAYER)
			send_packet(id, &pack);
	
	AddQuary(id, EV_UPDATE, name.c_str(), m_pass.c_str());
}

void CPlayer::Guard() {
	if (isGuard) return;
	isGuard = true;
	AddTimer(id, EV_GUARD_OFF, high_resolution_clock::now()+3s, NULL);
}

void CPlayer::TakeDamage(int damage) {
	healthPoint -= damage;
	SC_DAMAGED pack{sizeof(pack), sc_damaged, id, healthPoint};
	send_packet(id, &pack);
	for (auto& p : viewList)
		if (p <= MAX_PLAYER)
			send_packet(p, &pack);
	if (healthPoint <= 0) {
		healthPoint = 0;
		isAlive = false;
		cout << name << " 사망\n";
		SC_DEAD dp{ sizeof(dp), sc_dead, id };
		send_packet(id, &dp);
		for (auto& p : viewList)
			if (p <= MAX_PLAYER)
				send_packet(p, &pack);
	}
}

void CPlayer::Evade() {
	if (isEvade) return;
	isEvade = true;
	AddTimer(id, EV_EVADE_OFF, high_resolution_clock::now() + 2s, NULL);
}

void CPlayer::FireBall() {
	if (isSpellFireball) return;
	if (magicPoint < 10) return;
	cout << name << "이 " << "10마나를 사용하여 파이어볼 사용" << endl;
	magicPoint -= 10;
	isSpellFireball = true;
	fireball.SetPosition(pos);
	fireball.SetVolume(Volume(100, 100, 100));
	fireball.SetRotation(rotation.y);
	fireball.SetOwnerID(id);
	SC_FIREBALL pack{ sizeof(SC_FIREBALL), sc_fireball, id };
	for (auto& p : viewList)
		if (p < MAX_PLAYER) send_packet(p, &pack);
	send_packet(id, &pack);
	AddTimer(id, EV_FIREBALL, high_resolution_clock::now() + 1s, NULL);
}

void CPlayer::Hide() {
	if (isHide) return;
	isHide = true;
	cout << name << "(이)가 은신을 사용\n";
	SC_HIDE pack{sizeof(SC_HIDE), sc_hide, id};
	for (auto& p : viewList)
		if (p < MAX_PLAYER) send_packet(p, &pack);
}

void CPlayer::HideOff() {
	if (!isHide) return;
	isHide = false;
	cout << name << "(이)가 은신을 해제\n";
	SC_HIDE_OFF pack{ sizeof(SC_HIDE_OFF), sc_hide_off, id };
	for (auto& p : viewList)
		if (p < MAX_PLAYER) send_packet(p, &pack);
}

void CPlayer::Assassinate() {
	if (isAssasinate) return;
	if (!isHide) return;
	Sleep(500);
	for (auto& oid : viewList) {
		if (oid < NPC_ID_START) continue;
		if (GetDistance(g_monster[oid]->GetPosition()) < ATTACK_RANGE) {
			cout << name << "의 암살 스킬로 몬스터" << oid << "에게 " << atkPoint*2 << "의 대미지!" << endl;
			if (g_monster[oid]->TakeDamage(atkPoint*2) <= 0)
				KillMonster(oid);
			isHide = false;
		}
	}
	SC_ASSASSIN pack{ sizeof(SC_ASSASSIN), sc_assassin, id };
	for (auto& p : viewList)
		if (p < NPC_ID_START) send_packet(p, &pack);
	send_packet(id, &pack);
	AddTimer(id, EV_ASS_OFF, high_resolution_clock::now() + 2s, NULL);
}

void CPlayer::WeaponOn(const Weapon_Type& wpn) {
	if (wpnType != wpn_none) return;
	if (wpn == wpn_sword)
		cout << name << "(이)가 검, 방패 장착\n";
	else if (wpn == wpn_hammer)
		cout << name << "(이)가 해머 장착\n";
	wpnType = wpn;

	if (wpnType == wpn_sword) {
		SC_SWORD_ON pack{ sizeof(SC_SWORD_ON), sc_sword_on, id };
		for (int i = 0; i < MAX_PLAYER; ++i)
			send_packet(i, &pack);
	}
	else if (wpnType == wpn_hammer) {
		SC_HAMMER_ON pack{ sizeof(SC_HAMMER_ON), sc_hammer_on, id };
		for (int i = 0; i < MAX_PLAYER; ++i)
			send_packet(i, &pack);
	}
}

void CPlayer::WeaponOff() {
	if (wpnType == wpn_none) return;
	cout << name << "(이)가 무기를 탈착\n";
	if (wpnType == wpn_sword) {
		SC_SWORD_OFF pack{ sizeof(SC_SWORD_OFF), sc_sword_off, id };
		for (int i = 0; i < MAX_PLAYER; ++i)
			send_packet(i, &pack);
	}
	else if (wpnType == wpn_hammer) {
		SC_HAMMER_OFF pack{ sizeof(SC_HAMMER_OFF), sc_hammer_off, id };
		for (int i = 0; i < MAX_PLAYER; ++i)
			send_packet(i, &pack);
	}
	wpnType = wpn_none;
}

void CPlayer::MoveTo(const Position& p) {
	SetPosition(p);
	unordered_set<int> oldVl = viewList; // 현재 뷰리스트 카피
	unordered_set<int> newVl;
	// 모든 플레이어를 검사하여 시야에 있으면 새로운 뷰 리스트에 넣는다
	for (int i = 0; i < MAX_CLIENTS; ++i) {
		if (g_player[i] == NULL) continue;
		if (g_player[i]->state != In_Game) continue;
		// if (GetDistance(g_player[i]->pos) > MAX_VIEW_RANGE) continue;	// 일단 시야 빼두자
		if (i == id) continue;
		newVl.insert(i);
	}
	for (int i = NPC_ID_START; i < START_POINT_MONSTER+MAX_MONSTER; ++i) {
		// cout << i << endl;
		if (g_monster[i] == NULL) continue;
		if (GetDistance(g_monster[i]->GetPosition()) > MAX_VIEW_RANGE) continue;
		newVl.insert(i);	// player index와 구분한다
	}
	// if (boss != NULL || GetDistance(boss->GetPosition()))
	// 	newVl.insert(BOSS_IDX);	// Boss를 얼마나 스폰시킬지 몰라서 일단 단일개체로 설정함
	/// 일단 보스빼고 송수신해보자

	// send_packet(id, &MakeUpdatePacket());

	// 시야에 새로 들어온 오브젝트에 대한 처리
	for (auto& no : newVl) {
		if (oldVl.count(no) == 0) {	// 기존 뷰리스트엔 x 새 뷰리스트에 in
			// no enter packet to me
			if (no < MAX_PLAYER)
				send_packet(id, &g_player[no]->MakeUpdatePacket()); // Enter
			else if (no >= 20000) continue;	// Boss
			else {
				send_packet(id, &g_monster[no]->MakeUpdatePacket());
				AddTimer(no, EV_MONSTER, chrono::high_resolution_clock::now() + 1s, NULL);
			}
			viewList.insert(no);	// 뷰리스트에 추가
			if (!(no < MAX_PLAYER)) continue;
			g_player[no]->pLock.lock();
			if (g_player[no]->viewList.count(id) == 0) {
				// my enter packet to no 
				g_player[no]->pLock.unlock();
				send_packet(no, &MakeUpdatePacket());
				g_player[no]->viewList.insert(id);
			}
			else {
				g_player[no]->pLock.unlock();
				send_packet(no, &MakeUpdatePacket());
				// my move packet to no
			}
		}
		else {
			// 기존 뷰리스트에도 in
			if (!(no < MAX_PLAYER)) continue;
			g_player[no]->pLock.lock();
			if (g_player[no]->viewList.count(id) != 0) {
				g_player[no]->pLock.unlock();
				send_packet(no, &MakeUpdatePacket());
				// my move packet to no
			}
			else {
				g_player[no]->pLock.unlock();
				send_packet(no, &MakeUpdatePacket());
				g_player[no]->viewList.insert(id);
				// my enter packet ro no
			}
		}
	}

	// // 시야에서 벗어났을 때
	// for (auto& oo : oldVl) {
	// 	if (newVl.count(oo) == 0) {
	// 		send_packet(id, &g_player[oo]->MakeLeavePacket());
	// 		// oo leave packet to me
	// 		if (!(oo < MAX_PLAYER)) continue;
	// 		g_player[oo]->pLock.lock();
	// 		if (g_player[oo]->viewList.count(id) != 0) {
	// 			g_player[oo]->pLock.unlock();
	// 			send_packet(oo, &MakeLeavePacket());
	// 			// my leave packet to oo
	// 		}
	// 		else g_player[oo]->pLock.unlock();
	// 	}
	// }
}

void CPlayer::SetPosition(const Position& pos) {
	// if (GetDistance(pos) < 10 )
	this->pos = pos;
}
Position CPlayer::GetPosition() const {
	return pos;
}
int CPlayer::GetDistance(Position pos) {
	int x = pow((this->pos.x - pos.x), 2);
	int y = pow((this->pos.y - pos.y), 2);
	// int z = pow((this->pos.z - pos.z), 2);

	int distance = sqrt(x + y);
	return abs(distance);
}

SC_OBJECT_ENTER CPlayer::MakeEnterPacket() {
	SC_OBJECT_ENTER p;
	p.type = sc_enter_obj;
	p.size = sizeof(SC_OBJECT_ENTER);
	p.o_type = OBJ_PLAYER;
	strcpy_s(p.name, name.c_str());
	p.pos = pos;
	p.oid = id;
	return p;
}

SC_UPDATE_OBJ CPlayer::MakeUpdatePacket() {
	SC_UPDATE_OBJ p;
	p.type = sc_update_obj;
	p.size = sizeof(SC_UPDATE_OBJ);
	p.pos = pos;
	p.oid = id;
	p.rotation = rotation;
	p.velocity = velocity;
	return p;
}

SC_OBJECT_LEAVE CPlayer::MakeLeavePacket() {
	SC_OBJECT_LEAVE p;
	p.type = sc_leave_obj;
	p.size = sizeof(SC_OBJECT_LEAVE);
	p.id = id;
	return p;
}

SC_GET_EXP CPlayer::MakeGetExpPacket() {
	SC_GET_EXP p;
	p.type = sc_get_exp;
	p.size = sizeof(SC_GET_EXP);
	p.exp = exp;
	return p;
}

void CPlayer::EnterGame() {
	// auto myEnter = MakeEnterPacket();
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (id == i) continue;
		if (g_player[i] == NULL) continue;
		if (!g_player[i]->isAlive) continue;
		viewList.insert(i);
		g_player[i]->viewList.insert(id);
		send_packet(i, &MakeEnterPacket());
		send_packet(id, &g_player[i]->MakeEnterPacket());
	}
	//for (int i = NPC_ID_START; i < NPC_ID_START + MAX_MONSTER; ++i) {
	//	if (g_monster[i] == NULL) continue;
	//	// if (GetDistance(g_monster[i]->GetPosition()) < MAX_VIEW_RANGE)
	//	// send_packet(id, &g_monster[i]->MakeEnterPacket());
	//}
	// for (int i = 0; i < 4; ++i) {
	// 	int idx = NPC_ID_START + i * 1000;
	// 	viewList.insert(idx);
	// }
	// // if (GetDistance(boss->GetPosition()) < MAX_VIEW_RANGE)
	// send_packet(id, &boss->MakeEnterPacket());	
	viewList.insert(NPC_ID_START);
	viewList.insert(CYCLOPS_ID_START);
	viewList.insert(BEETLE_ID_START);
	viewList.insert(MINI_GOLEM_ID_START);
	isHost = false;
	if (id == 0) {	// 0이 아닌 경우엔 예외처리 나중에
		isHost = true;
		SC_SET_HOST pack;
		pack.size = sizeof(SC_SET_HOST);
		pack.type = sc_set_host;
		send_packet(id, &pack);
	}
}

void CPlayer::EnterObj(int oid) {
	pLock.lock();
	viewList.insert(oid);
	pLock.unlock();
}

void CPlayer::SetRotation(const Position& rotation) {
	this->rotation = rotation;
}

void CPlayer::SetName(const char* name) {
	this->name = {name};
}

void CPlayer::SetPass(const char* pass) {
	m_pass = { pass };
}

int CPlayer::GetMaxHP() const {
	return MaxHP;
}

int CPlayer::GetMaxMP() const {
	return MaxMP;
}

int CPlayer::GetEXP() const {
	return exp;
}

int CPlayer::GetLevel() const {
	return level;
}

void CPlayer::SetVelocity(const Position& v) {
	velocity = v;
}