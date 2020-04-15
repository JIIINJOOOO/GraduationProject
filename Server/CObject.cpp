#include "CObject.h"
#include <math.h>
void CObject::Initialize(Position pos, Velocity vel, Volume vol, Accel acc, int type) {
	SetPosition(pos);
	SetVelocity(vel);
	SetVolume(vol);
	SetAccel(acc);
	m_type = type;
}

// Getter & Setter
void CObject::SetPosition(const Position& pos) {
	m_pos = pos;
}
void CObject::SetVelocity(const Velocity& vel) {
	m_vel = vel;
}
void CObject::SetVolume(const Volume& vol) {
	m_vol = vol;
}
void CObject::SetAccel(const Accel& acc) {
	m_acc = acc;
}
Position CObject::GetPosition() const {
	return m_pos;
}
Velocity CObject::GetVelocity() const {
	return m_vel;
}
Volume CObject::GetVolume() const {
	return m_vol;
}
Accel CObject::GetAccel() const {
	return m_acc;
}
int CObject::GetDeleteCoolTime() const {
	return m_deleteCoolTime;
}
int CObject::GetDistance(const Position& pos) {
	int x = pow((m_pos.x - pos.x), 2);
	int y = pow((m_pos.y - pos.y), 2);
	int z = pow((m_pos.z - pos.z), 2);

	int distance = sqrt(x + y + z);
	return abs(distance);
}
int CObject::GetType() const {
	return m_type;
}