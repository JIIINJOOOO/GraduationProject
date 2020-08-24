#pragma once
#include "BossState.h"
#include "CPathFinder.h"
#include "protocol.h"

#define MAX_HAND_HP 100
#define MAX_LEG_HP 100
#define MAX_HEAD_HP 100
#define MAX_BODY_HP 100

struct HealthPoint {
	unsigned short head_hp;
	unsigned short right_hand_hp;
	unsigned short left_hand_hp;
	unsigned short right_leg_hp;
	unsigned short left_leg_hp;
	unsigned short body_hp;
};

struct IsDestroy {
	bool rightLeg;
	bool leftLeg;
	bool rightHand;
	bool leftHand;
	bool head;
	bool body;
};
class BossState;

class Boss{
private:
	HealthPoint healthPoint;
	Position pos, defaultPos;
	IsDestroy isDestroy;
	BossState* state;
	CPathFinder* pathFinder;
	list<POS_2D> chasePath;
	bool isActive;
	int target;
	int id;

public:
	Boss() = default;
	~Boss() = default;

	void Initialize(const int& id, const Position& p);
	void Respawn();
	void IsPartDestroyed();
	B_STATE Idle();
	B_STATE Chase();
	B_STATE Attack();
	void Update();

	void ChangeState(BossState* ns);
	IsDestroy GetIsDestroy() const;
	Position GetPosition() const;
	int GetTotalHP() const;

	SC_OBJECT_ENTER MakeEnterPacket();
	SC_UPDATE_OBJ MakeUpdatePacket();
};
