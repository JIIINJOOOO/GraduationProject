#pragma once
#include "Physics.h"

#define MAX_OBJ 200
#define MAX_MOVE_RANGE 30
enum Obj_Type {
	obj_player, obj_monster
};

class CObject {
private:
	Position m_pos;
	Velocity m_vel;
	Volume m_vol;
	Accel m_acc;
	int m_type;
	int m_deleteCoolTime;
public:
	CObject() = default;
	~CObject() = default;

	void Initialize(Position pos, Velocity vel, Volume vol, Accel acc, int type);

	// Getter & Setter
	void SetPosition(const Position& pos);
	void SetVelocity(const Velocity& vel);
	void SetVolume(const Volume& vol);
	void SetAccel(const Accel& acc);
	Position GetPosition() const;
	Velocity GetVelocity() const;
	Volume GetVolume() const;
	Accel GetAccel() const;
	int GetDeleteCoolTime() const;
	int GetType() const;

	int GetDistance(const Position& pos);
};

