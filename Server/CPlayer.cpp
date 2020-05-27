#include "CPlayer.h"
#include "packet.h"
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
	m_level = p.m_level;
	pos = { 0,0,0 };
}
bool CPlayer::operator== (const CPlayer& cp) const {
	return m_id == cp.m_id;
}

void CPlayer::SetState(const int& state) {
	m_state = state;
}
int CPlayer::GetState() {
	return m_state;
}
std::string CPlayer::GetID() {
	return m_id;
}
int CPlayer::GetRoomNum() {
	return m_roomNum;
}
void CPlayer::Ready() {
	m_state = ready;
	printf("%s is ready!\n", m_id.c_str());
	if (m_ready) m_ready = false;
	else m_ready = true;
}
void CPlayer::Attck() {
	
}

void CPlayer::SetPosition(const Position& pos) {
	if (GetDistance(pos) < 10)
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