#include <map>
#include <random>
#include "Event.h"
#include "CMonster.h"
#include "globals.h"
#include "CTerrain.h"
#include "Boss.h"
#define RECOVERY_TIME 600	// 60프레임 * 10s
#define RECOVERY_POINT 1

#define GOBLIN_0 10000
#define GOBLIN_1 10001
#define GOBLIN_2 10002
#define GOBLIN_3 10003
#define GOBLIN_4 10004

#define CYCLOPS_0 10100
#define CYCLOPS_1 10101
#define CYCLOPS_2 10102

#define BEETLE_0 10200
#define BEETLE_1 10201

#define MINI_GOLEM_0 10300
#define MINI_GOLEM_1 10301
#define MINI_GOLEM_2 10302
#define MINI_GOLEM_3 10303
#define MINI_GOLEM_4 10304
#define MINI_GOLEM_5 10305

#define LAZARD_0 10400
#define LAZARD_1 10401
#define LAZARD_2 10402

#define TROLL_0 10500
#define TROLL_1 10501


#define SPEED 0.03
#define EV_MONSTER 3

using namespace std;
// extern short board[SIDE_LEN][SIDE_LEN];
extern CTerrain *g_tmap;
// extern CLIENT g_clients[MAX_PLAYER];
extern map<int, CObject*> g_obj;
extern map<int, CMonster*> g_monster;
extern map<int, CPlayer*> g_player;
extern Boss* boss;
extern map<int, Boss*> g_boss;

extern void send_packet(int uid, void* p);

random_device rd;
mt19937 gen(rd());
uniform_int_distribution<int> dis(0, 2); // 0,1,2

float GetDegree(Position p1, Position p2) {
	return (atan2f(p2.y - p1.y, p2.x - p1.x) * 180) / Pi;
}

void CMonster::Chase(const CPlayer& target) {
	velocity = target.GetPosition() - pos;
	velocity.z = 0;
	pos = pos + (velocity * SPEED);
	rotation.y = GetDegree(pos, target.GetPosition());
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &MakeUpdatePacket());
	}

	return;
	// ----------------------------------------------------
	m_chasePath.clear();
	POS_2D my = { (pos.x-SIDE_MIN)/POS_SCALE, (pos.y-SIDE_MIN)/POS_SCALE };
	Position pp = target.GetPosition();
	POS_2D other = { (pp.x - SIDE_MIN) / POS_SCALE, (pp.y - SIDE_MIN) / POS_SCALE };
	m_chasePath = m_pathFinder->GetPath(my, other);
	
	POS_2D p = m_chasePath.front();
	// pos = p;
	// 임시로 좌표에 그대로 넣었는데 노드의 좌표이므로 사이 보정 해줘야함
	// 이걸 어케함 ㅆ1벌
	velocity.x = (p.x*POS_SCALE) + SIDE_MIN - pos.x;
	velocity.y = (p.y*POS_SCALE) + SIDE_MIN - pos.y;
	velocity.z = 0;
	// velocity.x = pp.x - pos.x;
	// velocity.y = pp.y - pos.y;
	auto dest_x = (p.x*POS_SCALE) + SIDE_MIN;
	auto dest_y = (p.y*POS_SCALE) + SIDE_MIN;
	// pos.x = pos.x + ((dest_x - pos.x) * 0.2);
	// pos.y = pos.y + ((dest_y - pos.y) * 0.2);
	pos.x = dest_x;
	pos.y = dest_y;

	m_chasePath.pop_front();
	
	// rotation.y = GetDegree(pos, Position(dest_x, dest_y, 0));
	rotation.y = GetDegree(pos, pp);
	// rotation.y = (int)(rotation.y + 360) % 360;
	// cout << "rotation  :  " << rotation.y << endl;
	// cout << pos.x <<"\t"<< pos.y << endl;
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &MakeUpdatePacket());
	}
	// cout << rotation << endl; 18
}

void CMonster::Idle() {
	
}

void CMonster::Attack(CPlayer& target) {
	/*
	응랑ㄱ 공격을 어떻게 홰야ㅏ하지?
	으어먀ㅐㄱㄹㄴㅇ
	*/
	// 949000.0
	int attack_num = dis(gen);	// 어떤 공격 사용할지인데 클라에서 출력만 다르게 하고 대미지 처리는 모두 동일하게 진행
	if (GetDistance(target.GetPosition()) > ATTACK_RANGE) return;
	if (GetDistance(target.GetPosition()) <= 0) return;
	rotation.y = GetDegree(pos, target.GetPosition());
	SC_OBJ_ATTACK pack{ sizeof(pack), sc_attack, id,attack_num };
	SC_SET_ROTATION pack_rot{ sizeof(pack_rot), sc_set_rotation, id };
	pack_rot.rot = rotation;
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &pack_rot);
		send_packet(i, &pack);
	}
	// if (!IsFront(target.GetPosition())) return;
	cout << "몬스터" << id << "(이)가 " << target.GetID() << target.GetIdx() << "를 공격 ";
	Sleep(500);
	if (GetDistance(target.GetPosition()) < GUARD_RANGE) {
		if (target.isGuard) {
			cout << target.GetID() << target.GetIdx() << "의 방어" << endl;
			// SC_OBJ_GUARD gp{ sizeof(gp), sc_guard, id };
			SC_BLOCK bp{ sizeof(bp), sc_block, id };
			// send_packet(target.GetIdx(), &gp);
			for (int i = 0; i < MAX_PLAYER; ++i) {
				if (g_player[i] == NULL) continue;
				send_packet(i, &bp);
			}
			return;
		}
		else if (target.isEvade) {
			cout << target.GetID() << target.GetIdx() << "의 회피" << endl;
			return;
		}
	}
	
	SC_DAMAGED d_pack{ sizeof(SC_DAMAGED), sc_damaged, target.GetIdx(), target.GetHP()};
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		// cout << "player damaged" << endl;
		send_packet(i, &d_pack);
	}
	cout << target.GetID() << target.GetIdx() << "(은)는 " << atkPoint << "의 대미지를 입었다\n";
	target.TakeDamage(atkPoint);
	
}

void CMonster::Update() {

	state->Execute(this);
	
}

void CMonster::UpdateWithClient() {
	if (monState == M_CHASE) {
		if (target == -1) return;
		if (g_player[target] == NULL) return;
		velocity = g_player[target]->GetPosition() - pos;
		// cout << target << endl;
	}
	// cout << velocity.x << "\t" << velocity.y << endl;
}

void CMonster::Initialize(Position pos, int type) {
	// SetDefaultPos(pos);
	this->pos = pos;
	m_defaultPos = pos;
	SetActivityRange(ACTIVITY_RANGE);
	healthPoint = MONSTER_MAX_HP;
	if (type == OBJ_GOBLIN) healthPoint = 50;
	else healthPoint = 30;
	//  = 10;
	m_type = type;	// 몬스터 타입이긴한데 알고리즘 동일
	xIdx = pos.x;
	zIdx = pos.z;
	state = IdleState::GetInstance();
	m_pathFinder = new CPathFinder;
	exp = 10;
	m_activityRange = ACTIVITY_RANGE;
	atkPoint = 10;
	isActive = false;
	atkRange = ATTACK_RANGE;
	// if (type == OBJ_MINI_GOLEM) atkRange = 200;
	for (int i = 0; i < MAX_PLAYER; ++i)
		player_dir[i] = 999'999'999;
}

void CMonster::ChangeState(State* newState) {
	state->Exit(this);
	state = newState;
	state->Enter(this);
}

bool CMonster::IsindefPos() {
	if (GetDistance(m_defaultPos) < IDLE_RANGE)
		return true;
	return false;
}

void CMonster::SetPrevHealthPoint() {
	prevHealth = healthPoint;
}

int CMonster::TakeDamage(int atk_point) {
	healthPoint -= atk_point;
	if (healthPoint <= 0) {
		Death();
		return 0;
	}
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &MakeDamagedPacket());
	}
	return healthPoint;
}

void CMonster::Death() {
	healthPoint = 0;
	pos = { 0,0,0 };
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
	return healthPoint;
}
void CMonster::SetHealthPoint(const int& hp) {
	healthPoint = hp;
}
int CMonster::GetState() const {
	return m_state;
}

void CMonster::SetPosition(const Position& pos) {
	if (GetDistance(pos) < 10)
		this->pos = pos;
}
Position CMonster::GetPosition() const {
	return pos;
}
Position CMonster::GetDefPosition() const {
	return m_defaultPos;
}
int CMonster::GetDistance(Position pos) {
	int x = pow((this->pos.x - pos.x), 2);
	int y = pow((this->pos.y - pos.y), 2);
	// cout << "pos-----" << endl;
	// cout << this->pos.x << "\t" << this->pos.y << "\t" << this->pos.z << endl;
	// cout << pos.x << "\t" << pos.y << "\t" << pos.z << endl;

	int distance = sqrt(x + y);
	return abs(distance);
}

void CMonster::SetTarget(const int& t) {
	target = t;
}
int CMonster::GetTarget() const {
	return target;
}
int CMonster::GetID() const {
	return id;
}

int CMonster::GetPrevHealthPoint() const {
	return prevHealth;
}

int CMonster::GetHP() const {
	return healthPoint;
}

void CMonster::ResetRecoverCool() {
	recoveryCoolTime = 0;
}

bool CMonster::IsFront(const Position& player_pos) {
	/*
	오브젝트가 내 앞에 있는지 검사
	velocity를 기준으로 내 앞인지만 검사하고
	*/
	if (velocity.x > 0)
		if (player_pos.x < pos.x) return false;
	if (velocity.x < 0)
		if (player_pos.x > pos.x) return false;
	if (velocity.y > 0)
		if (player_pos.y < pos.y) return false;
	if (velocity.y < 0)
		if (player_pos.y > pos.y) return false;
	return true;
}

SC_OBJECT_ENTER CMonster::MakeEnterPacket() {
	SC_OBJECT_ENTER p;
	p.o_type = m_type; // 몬스터 타입
	p.pos = pos;
	p.size = sizeof(SC_OBJECT_ENTER);
	p.type = sc_enter_obj;
	p.oid = id;
	return p;
}

SC_OBJECT_LEAVE CMonster::MakeLeavePacket() {
	SC_OBJECT_LEAVE p;
	p.id = id;
	p.type = sc_leave_obj;
	p.size = sizeof(SC_OBJECT_LEAVE);
	return p;
}

SC_UPDATE_OBJ CMonster::MakeUpdatePacket() {
	SC_UPDATE_OBJ p;
	p.oid = id;
	p.pos = pos;
	p.size = sizeof(p);
	p.type = sc_update_obj;
	p.rotation = rotation;
	p.velocity = velocity;
	return p;
}

SC_DAMAGED CMonster::MakeDamagedPacket() {
	SC_DAMAGED p;
	p.size = sizeof(SC_DAMAGED);
	p.type = sc_damaged;
	p.oid = id;
	p.hp = healthPoint;
	return p;
}

void CMonster::SetIndex(const int& idx) {
	id = idx;
}

int CMonster::GetEXP() const {
	return exp;
}

void CMonster::SetVelocity(const Position& v) {
	velocity = v;
}
void CMonster::SetRotation(const Position& r) {
	rotation = r;
}

int CMonster::GetAtkRange() const {
	return atkRange;
}

void CMonster::UpdateTarget() {
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) {
			player_dir[i] = 999'999'999;
			continue;
		}
		player_dir[i] = GetDistance(g_player[i]->GetPosition());
	}
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		if (!g_player[i]->isAlive) continue;
		if (0 <= player_dir[i] && player_dir[i] < player_dir[target])
			target = i;
	}
}

void CreateMonsters(int num) {
	Position defPos{ 15580.f, 77800.f, -490.f };
	// 13068.541016  86198.484375 -448.772888
	//for (int i = 0; i < 4; ++i) {
	//	short idx = START_POINT_MONSTER + i*1000;
	//	// defPos.x = xdis(gen);
	//	// defPos.y = ydis(gen);
	//	// defPos.z = board[(int)defPos.x + X_SIDE][(int)defPos.y + Y_SIDE];
	//	g_monster[idx] = new CMonster;
	//	g_monster[idx]->Initialize(defPos, normal);
	//	g_monster[idx]->SetIndex(idx);
	//	// g_monster[idx];

	//	// m_kdTree->Insert(*m_obj[idx]);
	//	cout << "몬스터" << idx << " 스폰(" << defPos.x << ", " << defPos.y << ", " << defPos.z << ")\n";
	//	// break;
	//}

	// Boss
	short idx = BOSS_IDX;
	defPos = { 22976.53125, 35731.6875, -932.329895 };
	g_boss[idx] = new Boss;
	g_boss[idx]->Initialize(idx, defPos);
	cout<<"Boss Golem(id="<<idx<<") Spawn("<<defPos.x<<", " << defPos.y << ", " << defPos.z << ")\n";
	AddTimer(BOSS_IDX, 4, chrono::high_resolution_clock::now() + 1s, NULL);

	// Goblin
	SpawnMonster(GOBLIN_0, 15640.0, 79760.0, -428.114075, OBJ_GOBLIN);
	SpawnMonster(GOBLIN_1, 16180.0, 79150.0, -428.114075, OBJ_GOBLIN);
	SpawnMonster(GOBLIN_2, 14950.0, 80431.7, -426.609375, OBJ_GOBLIN);

	// Cyclops
	SpawnMonster(CYCLOPS_0, 18448.2, 90659.0, -550.0, OBJ_CYCLOPS);
	SpawnMonster(CYCLOPS_1, 16920.0, 91460.0, -550.0, OBJ_CYCLOPS);
	SpawnMonster(CYCLOPS_2, 18714.1, 91243.3, -550.0, OBJ_CYCLOPS);

	// Beetle
	SpawnMonster(BEETLE_0, 53864.5, 63020.0, -1161.325195, OBJ_BEETLE);
	SpawnMonster(BEETLE_1, 56264.5, 64160.0, -981.325195, OBJ_BEETLE);

	// Lazard
	SpawnMonster(LAZARD_0, 23560.0, 81780.0, -612.0, OBJ_LAZARD);
	SpawnMonster(LAZARD_1, 22800.0, 83120.0, -612.0, OBJ_LAZARD);
	SpawnMonster(LAZARD_2, 24350.0, 82480.0, -612.0, OBJ_LAZARD);

	// Mini Golem
	// SpawnMonster(MINI_GOLEM_0, 29130.6, 78242.9, -374.753357, OBJ_MINI_GOLEM);
	SpawnMonster(MINI_GOLEM_1, 34532.1, 58345.7, -898.869446, OBJ_MINI_GOLEM);
	SpawnMonster(MINI_GOLEM_2, 33689.3, 59456.5, -859.889099, OBJ_MINI_GOLEM);
	SpawnMonster(MINI_GOLEM_3, 8813.9, 86079.6, -203.521851, OBJ_MINI_GOLEM);
	SpawnMonster(MINI_GOLEM_4, 8949.7, 88621.6, 196.9, OBJ_MINI_GOLEM);
	SpawnMonster(MINI_GOLEM_5, 10237.7, 86787.7, 238.321106, OBJ_MINI_GOLEM);

	// Troll
	SpawnMonster(TROLL_0, 64227.8, 56500.0, -414.321045, OBJ_TROLL);
	SpawnMonster(TROLL_1, 65807.8, 56600.0, -294.321045, OBJ_TROLL);

}

void SpawnMonster(int id, float x, float y, float z, int type) {
	switch (type) {
	case OBJ_GOBLIN:
		cout << "Goblin(id=" << id << ") Spawn(" << x << ", " << y << ", " << z << ")\n";
		break;
	case OBJ_CYCLOPS:
		cout << "Cyclops(id=" << id << ") Spawn(" << x << ", " << y << ", " << z << ")\n";
		break;
	case OBJ_BEETLE:
		cout << "Beetle(id=" << id << ") Spawn(" << x << ", " << y << ", " << z << ")\n";
		break;
	case OBJ_MINI_GOLEM:
		cout << "MiniGolem(id=" << id << ") Spawn(" << x << ", " << y << ", " << z << ")\n";
		break;
	case OBJ_LAZARD:
		cout << "Lazard(id=" << id << ") Spawn(" << x << ", " << y << ", " << z << ")\n";
		break;
	case OBJ_TROLL:
		cout << "Troll(id=" << id << ") Spawn(" << x << ", " << y << ", " << z << ")\n";
		break;
	}
	g_monster[id] = new CMonster;
	g_monster[id]->Initialize(Position(x, y, z), type);
	g_monster[id]->SetIndex(id);
	AddTimer(id, EV_MONSTER, chrono::high_resolution_clock::now() + 1s, NULL);
}