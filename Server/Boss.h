#pragma once
#include "BossState.h"
#include "CPathFinder.h"
#include "protocol.h"
#include <mutex>
#define MAX_HAND_HP 100
#define MAX_LEG_HP 100
#define MAX_HEAD_HP 100
#define MAX_BODY_HP 100

#define NUM_PARTS 12

#define R_HAND 0
#define R_FOREARM 1
#define R_UPPERARM 2
#define R_FOOR 3
#define R_CALF 4
#define R_THIGHT 5
#define L_HAND 6
#define L_FOREARM 7
#define L_UPPERARM 8
#define L_FOOR 9
#define L_CALF 10
#define L_THIGHT 11

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
	Position rotation;
	IsDestroy isDestroy;
	BossState* state;
	CPathFinder* pathFinder;
	list<POS_2D> chasePath;
	bool isActive;
	int target;
	int id;
	short BoneMap[NUM_PARTS];	// 남은 피격횟수
	mutex boneLock;

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
	void BoneMapUpdate(char* new_boneMap);

	void ChangeState(BossState* ns);
	IsDestroy GetIsDestroy() const;
	Position GetPosition() const;
	int GetTotalHP() const;

	void SetPosition(const Position& p);
	void SetRotation(const Position& p);

	SC_OBJECT_ENTER MakeEnterPacket();
	SC_UPDATE_OBJ MakeUpdatePacket();
};
