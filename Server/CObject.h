#pragma once
#include "Physics.h"

#define MAX_OBJ 200
#define MAX_MOVE_RANGE 30
#define FIREBALL_RANGE 500 
#define OBJ_ID_START 1000

enum Obj_Type {
	obj_player, obj_monster
};

class CObject {
private:
	Position pos{};
	Position defPos{};
	Velocity m_vel{};
	Volume vol{};
	Accel m_acc{};
	int m_type;
	int m_deleteCoolTime;
	float rotation;
	float speed;
	int life;
	int ownerID;
	int damage;
	int id;
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
	void SetRotation(const float& r);
	int GetDistance(const Position& pos);
	void SetOwnerID(const int& id);
	void Update();
	bool CollideTest(const Position& other);
};

