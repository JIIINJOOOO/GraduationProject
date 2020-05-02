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
