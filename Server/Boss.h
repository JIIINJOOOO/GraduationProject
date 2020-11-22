#pragma once
#include "BossState.h"
#include "CPathFinder.h"
#include "protocol.h"
#include <mutex>
#define MAX_HAND_HP 100
#define MAX_LEG_HP 100
#define MAX_HEAD_HP 100
#define MAX_BODY_HP 100
#define MAX_CALF_HP 100
#define MAX_FOREARM_HP 100
#define MAX_CALF_HP 100
#define MAX_FOOT_HP 100
#define MAX_THIGH_HP 100
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

#define BOSS_ACTIVITY_RANGE 4000
#define B_LONG_ATK_RANGE 1500

struct HealthPoint {
	short head_hp;
	short body_hp;

	short right_hand_hp;
	short right_forearm_hp;
	short right_upperarm_hp;
	short right_foot_hp;
	short right_thigh_hp;
	short right_calf_hp;

	short left_hand_hp;
	short left_forearm_hp;
	short left_upperarm_hp;
	short left_foot_hp;
	short left_thigh_hp;
	short left_calf_hp;
};

struct IsDestroy {
	bool rightHand;
	bool leftHand;
	bool head;

	bool rightForearm;
	bool rightUpperarm;
	bool rightFoot;
	bool rightThigh;
	bool rightCalf;

	bool leftForearm;
	bool leftUpperarm;
	bool leftFoot;
	bool leftThigh;
	bool leftCalf;

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
	int mainHP;

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
	int GetMainHP() const;

	void SetPosition(const Position& p);
	void SetRotation(const Position& p);

	SC_OBJECT_ENTER MakeEnterPacket();
	SC_UPDATE_OBJ MakeUpdatePacket();

	int GetTarget() const;
	int GetID() const;


	void BoneBreak(const int& part);
	void TakeDamage(const int& damage);

	bool IsLegBroken() const;
	bool IsRHandBroken() const;
	bool IsLHandBroken() const;
};
