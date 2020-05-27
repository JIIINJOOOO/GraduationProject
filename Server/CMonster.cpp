#include "CMonster.h"
#include "globals.h"
#include "CServer.h"
#include "CTerrain.h"
#include "main.h"

#define RECOVERY_TIME 600	// 60프레임 * 10s
#define RECOVERY_POINT 1
using namespace std;
extern short board[SIDE_LEN][SIDE_LEN];
extern CTerrain *g_tmap;
extern CLIENT g_clients[MAX_PLAYER];
extern map<int, CObject*> g_obj;
extern map<int, CMonster*> g_monster;
extern map<int, CPlayer*> g_player;

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
		// cout << "설마 돌아가고 있는거니?\n";
		m_chasePath = m_pathFinder->GetPath(myPos, otherPos);
		// cout << "에이 설마\n";

		for (int i = 0; i < m_chasePath.size(); ++i) {
			// myPos = m_chasePath.pop_front();
			// m_chasePath.
			myPos = m_chasePath.front();
			// cout << myPos.x << "\t" << myPos.y << endl;
			// 여기서는 각 타일 사이 이동 (100x100)을 보정해주어야함


			float z = g_tmap->OnGetHeight(myPos.x - SIDE_MIN, myPos.y - SIDE_MIN);
			// myPos.z = board[(int)myPos.x+X_SIDE][(int)myPos.y+Y_SIDE];
			myPos.z = z;
			m_chasePath.pop_front();
			Sleep(1000 / 400);
			// if (board[(int)myPos.x + X_SIDE][(int)myPos.y + 100] > 240)
				// printf("%f  %f  %f \n", myPos.x, myPos.y, myPos.z);
		}
		m_chasePath.clear();

		// float x_dis = myPos.x - otherPos.x;
		// float y_dis = myPos.y - otherPos.y;
		// x_dis=myPos.x + (-x_dis / sqrt((x_dis*x_dis) + (y_dis*y_dis)))*3.f;
		// y_dis=myPos.y + (-y_dis / sqrt((x_dis*x_dis) + (y_dis*y_dis)))*3.f;
		// 
		// float desZ = board[(int)x_dis + X_SIDE][(int)y_dis + Y_SIDE];
		// if (desZ > (myPos.z + 20.f))
		// 	return;
		// myPos.x = x_dis;
		// myPos.y = y_dis;
		// myPos.z = desZ;
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

void CMonster::Chase(const CPlayer& target) {
	if (m_chasePath.size() == 0)
		m_chasePath = m_pathFinder->GetPath(pos, target.GetPosition());
	Position p = m_chasePath.front();
	p.z = 0;	// z값으로 만든 2D배열에서 값을 가져와야 하는데 지금 터레인 에러나서 임시로 0해둠
	pos = p;
	m_chasePath.pop_front();
}

void CMonster::Idle() {
	recoveryCoolTime++;
	if (recoveryCoolTime > RECOVERY_TIME) {
		recoveryCoolTime = 0;
		if (m_healthPoint <= MONSTER_MAX_HP)
			m_healthPoint++;
	}
}

void CMonster::Attack(const CPlayer& target) {
	/*
	응랑ㄱ 공격을 어떻게 홰야ㅏ하지?
	으어먀ㅐㄱㄹㄴㅇ
	*/
}

void CMonster::Update() {
	state->Execute(this);
}

void CMonster::Initialize(Position pos, int type) {
	SetDefaultPos(pos);
	this->pos = pos;
	SetActivityRange(ACTIVITY_RANGE);
	m_healthPoint = MONSTER_MAX_HP;
	m_type = type;
	xIdx = pos.x;
	zIdx = pos.z;
	state = IdleState::GetInstance();
	m_pathFinder = new CPathFinder;
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
	prevHealth = m_healthPoint;
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
void CMonster::SetHealthPoint(const int& hp) {
	m_healthPoint = hp;
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
	int z = pow((this->pos.z - pos.z), 2);

	int distance = sqrt(x + y + z);
	return abs(distance);
}

void CMonster::SetTarget(const int& t) {
	target = t;
}
int CMonster::GetTarget() const {
	return target;
}
int CMonster::GetID() const {
	return m_myIdx;
}

int CMonster::GetPrevHealthPoint() const {
	return prevHealth;
}

void CMonster::ResetRecoverCool() {
	recoveryCoolTime = 0;
}


void MonsterThread() {
	CreateMonster(MAX_MONSTER);
	while (true) {
		for (int i = 0; i < MAX_MONSTER; ++i) {
			if (g_monster[i] == NULL) continue;
			g_monster[i]->Update();
			SC_UPDATE_OBJ pack;
			pack.size = sizeof(SC_UPDATE_OBJ);
			pack.type = sc_update_obj;
			pack.pos = g_monster[i]->GetPosition();
			pack.oid = i;
			for (auto& cl : g_clients)
				if (cl.isconnected && g_player[cl.id]!=NULL)
					if (g_monster[i]->GetDistance(g_player[cl.id]->GetPosition()) < MAX_VIEW_RANGE)
						send_packet(cl.id, &pack);
					
		}
		Sleep(1000 / 60);
	}
}

void CreateMonster(int num) {
	Position defPos{ 15455.5f,75128.1f,-40002.3f };
	for (int i = 0; i < num; ++i) {
		short idx = START_POINT_MONSTER + i;
		// defPos.x = xdis(gen);
		// defPos.y = ydis(gen);
		// defPos.z = board[(int)defPos.x + X_SIDE][(int)defPos.y + Y_SIDE];
		g_monster[i] = new CMonster;
		g_monster[i]->Initialize(defPos, normal);
		

		// m_kdTree->Insert(*m_obj[idx]);
	}
	cout << "Create Monsters Complete (num of : " << num << ")\n";
}