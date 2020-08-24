#include "CPlayer.h"
#include "CMonster.h"
#include "Boss.h"
#include "protocol.h"
#include <map>

extern map<int, CPlayer*> g_player;
extern map<int, CMonster*> g_monster;
extern Boss *boss;

extern void send_packet(int uid, void* p);

CPlayer::CPlayer(std::string id, std::string pass) {
	m_id = id; 
	m_pass = pass;
}
CPlayer::~CPlayer() {
	m_id.clear();
	m_pass.clear();
}
void CPlayer::Initialize(const CPlayer& p) {
	m_id = p.m_id;
	m_pass = p.m_pass;
	level = p.level;
	pos = { 14410.0f,77670.0f, -450.0f };
	/*
	28600.0
	77420.0
	-39726.15625
	*/
}

void CPlayer::Initialize(int hp, int o_type, int exp, int lv, int mp, int atk, int x, int y){
	healthPoint = hp;
	objType = o_type;
	this->exp = exp;
	level = lv;
	magicPoint = mp;
	atkPoint = atk;
	pos.x = x;
	pos.y = y;
	
}

bool CPlayer::operator== (const CPlayer& cp) const {
	return m_id == cp.m_id;
}

void CPlayer::SetState(const int& state) {
	this->state = state;
}
int CPlayer::GetState() {
	return state;
}
std::string CPlayer::GetID() {
	return m_id;
}
int CPlayer::GetRoomNum() {
	return m_roomNum;
}
void CPlayer::Ready() {
	// state = ready;
	if (isReady) isReady = false;
	else isReady = true;
}
void CPlayer::Attck() {
	
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
		if (i == m_idx) continue;
		newVl.insert(i);
	}
	for (int i = START_POINT_MONSTER; i < START_POINT_MONSTER+MAX_MONSTER; ++i) {
		// cout << i << endl;
		if (g_monster[i] == NULL) continue;
		if (GetDistance(g_monster[i]->GetPosition()) > MAX_VIEW_RANGE) continue;
		newVl.insert(i);	// player index와 구분한다
		// PSCS
	}
	if (boss != NULL || GetDistance(boss->GetPosition()))
		newVl.insert(BOSS_IDX);	// Boss를 얼마나 스폰시킬지 몰라서 일단 단일개체로 설정함
	/// 일단 보스빼고 송수신해보자

	// send_packet(m_idx, &MakeUpdatePacket());

	// 시야에 새로 들어온 오브젝트에 대한 처리
	for (auto& no : newVl) {
		if (oldVl.count(no) == 0) {	// 기존 뷰리스트엔 x 새 뷰리스트에 in
			// no enter packet to me
			if (no < MAX_PLAYER)
				send_packet(m_idx, &g_player[no]->MakeUpdatePacket()); // Enter
			else if (no >= 20000) continue;	// Boss
			else send_packet(m_idx, &g_monster[no]->MakeUpdatePacket());
			viewList.insert(no);	// 뷰리스트에 추가
			if (!(no < MAX_PLAYER)) continue;
			g_player[no]->pLock.lock();
			if (g_player[no]->viewList.count(m_idx) == 0) {
				// my enter packet to no 
				g_player[no]->pLock.unlock();
				send_packet(no, &MakeUpdatePacket());
				g_player[no]->viewList.insert(m_idx);
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
			if (g_player[no]->viewList.count(m_idx) != 0) {
				g_player[no]->pLock.unlock();
				send_packet(no, &MakeUpdatePacket());
				// my move packet to no
			}
			else {
				g_player[no]->pLock.unlock();
				send_packet(no, &MakeUpdatePacket());
				g_player[no]->viewList.insert(m_idx);
				// my enter packet ro no
			}
		}
	}

	// // 시야에서 벗어났을 때
	// for (auto& oo : oldVl) {
	// 	if (newVl.count(oo) == 0) {
	// 		send_packet(m_idx, &g_player[oo]->MakeLeavePacket());
	// 		// oo leave packet to me
	// 		if (!(oo < MAX_PLAYER)) continue;
	// 		g_player[oo]->pLock.lock();
	// 		if (g_player[oo]->viewList.count(m_idx) != 0) {
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
	int z = pow((this->pos.z - pos.z), 2);

	int distance = sqrt(x + y + z);
	return abs(distance);
}

SC_OBJECT_ENTER CPlayer::MakeEnterPacket() {
	SC_OBJECT_ENTER p;
	p.type = sc_enter_obj;
	p.size = sizeof(SC_OBJECT_ENTER);
	p.o_type = OBJ_PLAYER;
	strcpy_s(p.name, m_id.c_str());
	p.pos = pos;
	p.oid = m_idx;
	return p;
}

SC_UPDATE_OBJ CPlayer::MakeUpdatePacket() {
	SC_UPDATE_OBJ p;
	p.type = sc_update_obj;
	p.size = sizeof(SC_UPDATE_OBJ);
	p.pos = pos;
	p.oid = m_idx;
	p.rotation = rotation;
	return p;
}

SC_OBJECT_LEAVE CPlayer::MakeLeavePacket() {
	SC_OBJECT_LEAVE p;
	p.type = sc_leave_obj;
	p.size = sizeof(SC_OBJECT_LEAVE);
	p.id = m_idx;
	return p;
}

void CPlayer::EnterGame() {
	// auto myEnter = MakeEnterPacket();
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (m_idx == i) continue;
		if (g_player[i] == NULL) continue;
		viewList.insert(i);
		g_player[i]->viewList.insert(m_idx);
		send_packet(i, &MakeEnterPacket());
		send_packet(m_idx, &g_player[i]->MakeEnterPacket());
	}
	// for (int i = 0; i < MAX_MONSTER; ++i) {
	// 	if (m_idx == i) continue;
	// 	if (g_monster[i] == NULL) continue;
	// 	// if (GetDistance(g_monster[i]->GetPosition()) < MAX_VIEW_RANGE)
	// 	send_packet(m_idx, &g_monster[i]->MakeEnterPacket());
	// }
	// // // if (GetDistance(boss->GetPosition()) < MAX_VIEW_RANGE)
	// send_packet(m_idx, &boss->MakeEnterPacket());	

	if (m_idx == 0) {	// 0이 아닌 경우엔 예외처리 나중에
		SC_SET_HOST pack;
		pack.size = sizeof(SC_SET_HOST);
		pack.type = sc_set_host;
		send_packet(m_idx, &pack);
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