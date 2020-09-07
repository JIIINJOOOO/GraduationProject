#include "CObject.h"
#include <math.h>
#include "CMonster.h"
#include "CPlayer.h"
#include "Boss.h"
#include <map>
#include "Event.h"
#define EV_FIREBALL 12
extern map<int, CPlayer*> g_player;
extern map<int, CMonster*> g_monster;
extern Boss *boss;

extern void send_packet(int uid, void* p);

void CObject::Initialize(Position pos, Velocity vel, Volume vol, Accel acc, int type) {
	SetPosition(pos);
	SetVelocity(vel);
	SetVolume(vol);
	SetAccel(acc);
	m_type = type;
	speed = 1000.f;
	damage = 100;
}

// Getter & Setter
void CObject::SetPosition(const Position& pos) {
	// if (GetDistance(pos) < MAX_MOVE_RANGE)
	defPos = this->pos = pos;
}
void CObject::SetVelocity(const Velocity& vel) {
	m_vel = vel;
}
void CObject::SetVolume(const Volume& vol) {
	this->vol = vol;
}
void CObject::SetAccel(const Accel& acc) {
	m_acc = acc;
}
Position CObject::GetPosition() const {
	return pos;
}
Velocity CObject::GetVelocity() const {
	return m_vel;
}
Volume CObject::GetVolume() const {
	return vol;
}
Accel CObject::GetAccel() const {
	return m_acc;
}
int CObject::GetDeleteCoolTime() const {
	return m_deleteCoolTime;
}
int CObject::GetDistance(const Position& pos) {
	int x = pow((this->pos.x - pos.x), 2);
	int y = pow((this->pos.y - pos.y), 2);
	int z = pow((this->pos.z - pos.z), 2);

	int distance = sqrt(x + y + z);
	return abs(distance);
}
int CObject::GetType() const {
	return m_type;
}
void CObject::SetRotation(const float& r) {
	rotation = r;
}
void CObject::SetOwnerID(const int& id) {
	ownerID = id;
	this->id = OBJ_ID_START + ownerID;
}

void CObject::Update() {
	pos.x += cos(rotation)*speed;
	pos.y += sin(rotation)*speed;

	// Move 근데 파이어볼인데 무브가 맞는 말인가 이게 맞느남ㄹ이 아니면 뭐가 맞는 말일까 힇아랑ㄴ한ㅇㅎ
	SC_FIREBALL_MOVE mpack{ sizeof(SC_FIREBALL_MOVE), sc_fireball_move, ownerID, pos };
	mpack.rotation = Position(0, rotation, 0);	// 어차피 필요한건 y뿐이라 이거만 넘겨받았는데 힝힝
	for (auto& p : g_player[ownerID]->viewList)
		if (p < MAX_PLAYER) send_packet(p, &mpack);
	send_packet(ownerID, &mpack);

	for (auto& oid : g_player[ownerID]->viewList) {
		if (oid < NPC_ID_START) continue;
		if (oid != BOSS_IDX) 
			if (CollideTest(g_monster[oid]->GetPosition())) {
				cout << g_player[ownerID]->GetID() << "의 파이어볼이 몬스터" << oid << "에게 적중\n";
				if (g_monster[oid]->TakeDamage(damage) <= 0)
					g_player[ownerID]->KillMonster(oid);
				// Leave Packet
				SC_FIREBALL_OFF pack{ sizeof(SC_FIREBALL_OFF), sc_fireball_off, ownerID };
				for (auto& p : g_player[ownerID]->viewList)
					if (p < MAX_PLAYER) send_packet(p, &pack);
				send_packet(ownerID, &pack);
				g_player[ownerID]->isSpellFireball = false;
				return;
			}
		else CollideTest(boss->GetPosition());
	}

	if (GetDistance(defPos) > FIREBALL_RANGE) {
		cout << g_player[ownerID]->GetID() << "의 파이어볼이 소멸\n";
		g_player[ownerID]->isSpellFireball = false;
		return;
	}
	AddTimer(ownerID, EV_FIREBALL, high_resolution_clock::now() + 2ms, NULL);
}

bool CObject::CollideTest(const Position& other) {
	if (pos.x + vol.x < other.x) return false;
	if (pos.x - vol.x > other.x) return false;
	if (pos.y + vol.y < other.y) return false;
	if (pos.y - vol.y > other.y) return false;
	return true;
}