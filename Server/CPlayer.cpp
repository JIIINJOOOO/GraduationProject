#include "CPlayer.h"
#include "CMonster.h"
#include "Boss.h"
#include "protocol.h"
#include "Event.h"
#include <map>
#include "CDBConnector.h"


extern map<int, CPlayer*> g_player;
extern map<int, CMonster*> g_monster;
extern map<int, Boss*> g_boss;
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
	pos = { 10227.3125, 76509.90625, -437.344971 };
	/*
	10227.3125, 76509.90625, -437.344971
	*/
}

void CPlayer::Initialize(int hp, int o_type, int exp, int lv, int mp, int atk, int x, int y){
	MaxHP = healthPoint = hp;
	MaxMP = magicPoint = mp;
	objType = o_type;
	this->exp = exp;
	level = lv;
	atkPoint = atk;
	atkPoint = 10;
	pos.x = x;
	pos.y = y;
	pos = { 10227.3125, 76509.90625, -437.344971 };
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
	moveState = P_WALKING;
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

	// ������ �������κ��� 1�� �̳��� ���ݽ� �޺� ����
	if (lastAtkTime + 1s > high_resolution_clock::now()) {
		if (wpnType == wpn_sword)
			comboCnt = (comboCnt + 1) % MAX_SWORD_COMBO;
		else if (wpnType == wpn_hammer)
			comboCnt = (comboCnt + 1) % MAX_HAMMER_COMBO;
	}
	else comboCnt = 0;	// reset

	SC_OBJ_ATTACK pack{ sizeof(SC_OBJ_ATTACK), sc_attack, id, comboCnt };
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &pack);
	}
	// send_packet(id, &pack);
	// Sleep(500);
	this_thread::sleep_for(100ms);
	bool flag = false;
	for (int i = NPC_ID_START; i < NPC_ID_START + MAX_MONSTER; ++i) {
		// if (i < NPC_ID_START) continue;
		// cout << oid<<"\t"<<GetDistance(g_monster[oid]->GetPosition()) << endl;
		if (g_monster[i] == NULL) continue;
		if (g_monster[i]->GetHealthPoint() == 0) continue;
		if (GetDistance(g_monster[i]->GetPosition()) < ATTACK_RANGE) {
			// if (!IsFront(g_monster[i]->GetPosition())) continue;
			if (GetDistance(g_monster[i]->GetPosition()) < 0) continue;
			if (flag == false) {
				rotation.y = GetDegree(pos, g_monster[i]->GetPosition());
				SC_SET_ROTATION rp{ sizeof(SC_SET_ROTATION), sc_set_rotation, id, rotation };
				for (int i = 0; i < MAX_PLAYER; ++i) {
					if (g_player[i] == NULL) continue;
					send_packet(i, &rp);
				}
				flag = true;
			}
			Position directionVec = { cos(rotation.y), sin(rotation.y), 0 };

			cout << name << id << "�� �������� ����" << i << "���� " << atkPoint << "�� �����!" << endl;
			if (g_monster[i]->TakeDamage(atkPoint) <= 0)
				KillMonster(i);
			//	break;
		}
	}
	
	AddTimer(id, EV_ATK_OFF, high_resolution_clock::now() + 500ms, NULL);	// ���� ���Ḧ �˸�
}

void CPlayer::Berserk() {
	if (isBerserk) return;
	if (wpnType != wpn_sword) return;
	cout << name << id << "(��)�� ����ũ ��ų�� ���" << endl;
	
	SC_BERSERK pack{sizeof(SC_BERSERK), sc_berserk, id};
	for (auto& p : viewList)
		if (p < MAX_PLAYER) send_packet(p, &pack);
	this_thread::sleep_for(100ms);
	for (int i = NPC_ID_START; i < NPC_ID_START + MAX_MONSTER; ++i) {
		// if (oid < NPC_ID_START) continue;
		if (g_monster[i] == NULL) continue;
		if (g_monster[i]->GetHealthPoint() == 0) continue;
		if (GetDistance(g_monster[i]->GetPosition()) < BERSERK_RANGE) {
			if (GetDistance(g_monster[i]->GetPosition()) < 0) continue;
			if (g_monster[i]->TakeDamage(50) <= 0)
				KillMonster(i);
			cout << name << id << "�� ����ũ�� ����" << i << "���� " << 50 << "�� �����!" << endl;
		}
	}
	isBerserk = true;
	atkPoint += BERSERK_BONUS;
	magicPoint += BERSERK_BONUS;
	AddTimer(id, EV_BERSER, high_resolution_clock::now()+15s, 0); // �ð��� ���� ����
}

void CPlayer::BerserkOff() {
	isBerserk = false;
	atkPoint -= BERSERK_BONUS;
	magicPoint -= BERSERK_BONUS;
	cout << name << id << "�� ����ũ �ð��� ����" << endl;
}

void CPlayer::KillMonster(int oid) {
	cout << name << id << "(��)�� ����" << oid << "�� ���̰� ����ġ " << g_monster[oid]->GetEXP() << "ȹ��" << endl;
	exp += g_monster[oid]->GetEXP();
	send_packet(id, &MakeGetExpPacket());
	this_thread::sleep_for(500ms);
	SC_DEAD pack{ sizeof(SC_DEAD), sc_dead, oid };
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &pack);
	}
	if (exp >= 100) LevelUp();
}

void CPlayer::LevelUp() {
	++level;
	cout << name << id << " ������(" << level << ")\n";
	exp -= 100;
	atkPoint += LEVEL_UP_BONUS;
	MaxHP += LEVEL_UP_BONUS;
	MaxMP += LEVEL_UP_BONUS;
	healthPoint = MaxHP;
	magicPoint  = MaxMP;
	SC_LEVEL_UP pack{ sizeof(SC_LEVEL_UP), sc_level_up , id, level, exp, healthPoint, magicPoint };
	// send_packet(id, &pack);
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(id, &pack);
	}
	AddQuary(id, EV_UPDATE, name.c_str(), m_pass.c_str());
}

void CPlayer::Guard() {
	if (isGuard) return;
	isGuard = true;
	SC_OBJ_GUARD pack{ sizeof(SC_OBJ_GUARD), sc_guard, id };
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &pack);
	}
	AddTimer(id, EV_GUARD_OFF, high_resolution_clock::now()+3s, NULL);
}

void CPlayer::TakeDamage(int damage) {
	healthPoint -= damage;
	SC_DAMAGED pack{sizeof(pack), sc_damaged, id, healthPoint};
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &pack);
	}
	if (healthPoint <= 0) {
		healthPoint = 0;
		isAlive = false;
		cout << name << id << " ���\n";
		SC_DEAD dp{ sizeof(dp), sc_dead, id };
		for (int i = 0; i < MAX_PLAYER; ++i) {
			if (g_player[i] == NULL) continue;
			send_packet(i, &dp);
		}
	}
}

void CPlayer::Evade() {
	if (isEvade) return;
	isEvade = true;
	SC_EVADE pack{ sizeof(SC_EVADE), sc_evade, id };
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &pack);
	}
	AddTimer(id, EV_EVADE_OFF, high_resolution_clock::now() + 2s, NULL);
}

void CPlayer::FireBall() {
	if (isSpellFireball) return;
	if (magicPoint < 10) return;
	cout << name << id << "�� " << "10������ ����Ͽ� ���̾ ���" << endl;
	magicPoint -= 10;
	isSpellFireball = true;
	fireball.SetPosition(pos);
	fireball.SetVolume(Volume(100, 100, 100));
	fireball.SetRotation(rotation.y);
	fireball.SetOwnerID(id);
	SC_FIREBALL pack{ sizeof(SC_FIREBALL), sc_fireball, id };
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &pack);
	}
	AddTimer(id, EV_FIREBALL, high_resolution_clock::now() + 1s, NULL);
}

void CPlayer::Hide() {
	if (isHide) return;
	isHide = true;
	//cout << name << id << "(��)�� ������ ���\n";
	SC_HIDE pack{sizeof(SC_HIDE), sc_hide, id};
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &pack);
	}
}

void CPlayer::HideOff() {
	if (!isHide) return;
	isHide = false;
	//cout << name << id << "(��)�� ������ ����\n";
	SC_HIDE_OFF pack{ sizeof(SC_HIDE_OFF), sc_hide_off, id };
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &pack);
	}
}

void CPlayer::Assassinate() {
	if (isAssasinate) return;
	// if (!isHide) return;
	SC_ASSASSIN pack{ sizeof(SC_ASSASSIN), sc_assassin, id };
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &pack);
	}
	this_thread::sleep_for(100ms);
	for (int i = NPC_ID_START; i < NPC_ID_START + MAX_MONSTER; ++i) {
		if (g_monster[i] == NULL) continue;
		if (g_monster[i]->GetHealthPoint() == 0) continue;
		if (GetDistance(g_monster[i]->GetPosition()) < ATTACK_RANGE) {
			if (GetDistance(g_monster[i]->GetPosition()) < 0) continue;
			// if (!IsFront(g_monster[oid]->GetPosition())) continue;
			cout << name << id << "�� �ϻ� ��ų�� ����" << i << "���� " << atkPoint*2 << "�� �����!" << endl;
			if (g_monster[i]->TakeDamage(atkPoint*2) <= 0)
				KillMonster(i);
			isHide = false;
		}
	}
	
	// send_packet(id, &pack);
	AddTimer(id, EV_ASS_OFF, high_resolution_clock::now() + 2s, NULL);
}

void CPlayer::WeaponOn(const Weapon_Type& wpn) {
	// if (wpnType != wpn_none) return;
	// if (wpnType == wpn == wpn_sword){
	// 	WeaponOff(wpn_sword);
	// 	return;
	// }
	// if (wpnType == wpn == wpn_hammer) {
	// 	WeaponOff(wpn_hammer);
	// 	return;
	// }
	if (wpnType == wpn) {
		WeaponOff(wpn);
		return;
	}
	if (wpn == wpn_sword)
		cout << name << id << "(��)�� ��, ���� ����\n";
	else if (wpn == wpn_hammer)
		cout << name << id << "(��)�� �ظ� ����\n";
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

void CPlayer::WeaponOff(const Weapon_Type& wpn) {
	if (wpnType == wpn_none) {
		WeaponOn(wpn);
		return;
	}
	if (wpnType == wpn == wpn_sword) {
		SC_SWORD_OFF pack{ sizeof(SC_SWORD_OFF), sc_sword_off, id };
		for (int i = 0; i < MAX_PLAYER; ++i)
			send_packet(i, &pack);
	}
	else if (wpnType == wpn == wpn_hammer) {
		SC_HAMMER_OFF pack{ sizeof(SC_HAMMER_OFF), sc_hammer_off, id };
		for (int i = 0; i < MAX_PLAYER; ++i)
			send_packet(i, &pack);
	}
	else return;
	cout << name << id << "(��)�� ���⸦ Ż��\n";
	wpnType = wpn_none;
}

void CPlayer::MoveTo(const Position& p) {
	SetPosition(p);
	unordered_set<int> oldVl = viewList; // ���� �丮��Ʈ ī��
	unordered_set<int> newVl;
	// ��� �÷��̾ �˻��Ͽ� �þ߿� ������ ���ο� �� ����Ʈ�� �ִ´�
	for (int i = 0; i < MAX_CLIENTS; ++i) {
		if (g_player[i] == NULL) continue;
		if (g_player[i]->state != In_Game) continue;
		// if (GetDistance(g_player[i]->pos) > MAX_VIEW_RANGE) continue;	// �ϴ� �þ� ������
		if (i == id) continue;
		newVl.insert(i);
	}
	for (int i = NPC_ID_START; i < START_POINT_MONSTER+MAX_MONSTER; ++i) {
		// cout << i << endl;
		if (g_monster[i] == NULL) continue;
		if (GetDistance(g_monster[i]->GetPosition()) > MAX_VIEW_RANGE) continue;
		newVl.insert(i);	// player index�� �����Ѵ�
	}
	// if (boss != NULL || GetDistance(boss->GetPosition()))
	// 	newVl.insert(BOSS_IDX);	// Boss�� �󸶳� ������ų�� ���� �ϴ� ���ϰ�ü�� ������
	/// �ϴ� �������� �ۼ����غ���

	// send_packet(id, &MakeUpdatePacket());

	// �þ߿� ���� ���� ������Ʈ�� ���� ó��
	for (auto& no : newVl) {
		if (oldVl.count(no) == 0) {	// ���� �丮��Ʈ�� x �� �丮��Ʈ�� in
			// no enter packet to me
			if (no < MAX_PLAYER)
				send_packet(id, &g_player[no]->MakeUpdatePacket()); // Enter
			else if (no >= 20000) continue;	// Boss
			else {
				send_packet(id, &g_monster[no]->MakeUpdatePacket());
				if (g_monster[no]->isActive) break;
				AddTimer(no, EV_MONSTER, chrono::high_resolution_clock::now() + 1s, NULL);
				g_monster[no]->isActive = true;
			}
			viewList.insert(no);	// �丮��Ʈ�� �߰�
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
			// ���� �丮��Ʈ���� in
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

	// // �þ߿��� ����� ��
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

		if (g_player[i]->wpnType == wpn_sword) {
			SC_SWORD_ON pack{ sizeof(SC_SWORD_ON), sc_sword_on, i };
			send_packet(id, &pack);
		}
		else if (g_player[i]->wpnType == wpn_hammer) {
			SC_HAMMER_ON pack{ sizeof(SC_HAMMER_ON), sc_hammer_on, i };
			send_packet(id, &pack);
		}

		SC_UPDATE_MOVE_STATE pack{ sizeof(SC_UPDATE_MOVE_STATE), sc_update_move_state, i };
		pack.state = g_player[i]->moveState;
		send_packet(id, &pack);
	}
	//for (int i = NPC_ID_START; i < NPC_ID_START + MAX_MONSTER; ++i) {
	//	if (g_monster[i] == NULL) continue;
	//	// if (GetDistance(g_monster[i]->GetPosition()) < MAX_VIEW_RANGE)
	//	// send_packet(id, &g_monster[i]->MakeEnterPacket());
	//} -291.574951 -918.58136 -1410.189697
	// for (int i = 0; i < 4; ++i) {
	// 	int idx = NPC_ID_START + i * 1000;
	// 	viewList.insert(idx);
	// }
	// // if (GetDistance(boss->GetPosition()) < MAX_VIEW_RANGE)
	// send_packet(id, &boss->MakeEnterPacket());	
	// viewList.insert(NPC_ID_START);
	// viewList.insert(10000);
	for (int i = NPC_ID_START; i < NPC_ID_START + MAX_MONSTER; ++i) {
		if (g_monster[i] == NULL) continue;
		if (g_monster[i]->GetHealthPoint() <= 0) continue;
		send_packet(id, &g_monster[i]->MakeEnterPacket());
	}
	
	// for (int i = BOSS_IDX; i < BOSS_IDX + 2; ++i) {
	// 	if (g_boss[i] == NULL) continue;
	// 	send_packet(id, &g_boss[i]->MakeEnterPacket());
	// }
	// send_packet(id, &g_monster[11000]->MakeEnterPacket());

	// viewList.insert(CYCLOPS_ID_START);
	// viewList.insert(BEETLE_ID_START);
	// viewList.insert(MINI_GOLEM_ID_START);
	isHost = false;
	if (id == 0) {	// 0�� �ƴ� ��쿣 ����ó�� ���߿�
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

bool CPlayer::IsFront(const Position& mon_pos) {
	/*
	������Ʈ�� �� �տ� �ִ��� �˻� 
	velocity�� �������� �� �������� �˻��ϰ� 
	*/
	if (velocity.x > 0)
		if (mon_pos.x < pos.x) return false;
	if (velocity.x < 0)
		if (mon_pos.x > pos.x) return false;
	if (velocity.y > 0)
		if (mon_pos.y < pos.y) return false;
	if (velocity.y < 0)
		if (mon_pos.y > pos.y) return false;
	return true;
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

int CPlayer::GetHP() const {
	return healthPoint;
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
	if (v.IsZero()) return;
	velocity = v;
}

void CPlayer::SetMoveState(const Movement_State& new_state) {
	if (new_state == moveState) return;
	if (new_state == P_ONWALL)
		cout << name << id << "��(��) �Ŵ޸��� ����\n";
	if (moveState == P_ONWALL)
		cout << name << id << "��(��) �Ŵ޸��� ����\n";

	moveState = new_state;
	SC_UPDATE_MOVE_STATE pack{ sizeof(pack), sc_update_move_state, id };
	pack.state = new_state;
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &pack);
	}
}