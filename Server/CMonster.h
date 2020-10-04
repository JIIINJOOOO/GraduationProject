#pragma once
#include "CObject.h"
#include "CPlayer.h"
#include "CPathFinder.h"
#define MAX_MONSTER 10000
#define ACTIVITY_RANGE 4000'00
#define ATTACK_RANGE 300
#define CHASE_RANGE 1700
#define MONSTER_MAX_HP 100
#define BOSS_IDX 20'000
#define START_POINT_MONSTER 10000
#define IDLE_RANGE 5
#define NO_DETECTED -1
#define NPC_ID_START 10000

#define GOBLIN_ID_START 10000
#define CYCLOPS_ID_START 11000
#define BEETLE_ID_START 12000
#define MINI_GOLEM_ID_START 13000

enum monster_state {
	idle, attack, move, chase, return_home,
	normal
};
enum M_STATE {
	M_IDLE, M_CHASE, M_ATTACK, M_RHOME, M_DEAD
};


class State;

class CMonster
{
private:
	int id;
	int xIdx, zIdx;
	
	Position m_defaultPos;
	Position pos;
	float z_range{ 30.f };
	list<POS_2D> m_chasePath;
	CPathFinder *m_pathFinder;

	int m_activityRange;
	int m_state{idle};
	int healthPoint;
	int m_type;
	State *state;
	int target{ NO_DETECTED };
	int prevHealth;
	int recoveryCoolTime = 0;
	int exp;
	int atkPoint;

	Position rotation;
	Position velocity;

	int player_dir[MAX_PLAYER];
public:
	bool isActive;
	M_STATE monState;
	Position chaseEndPos;
	CMonster() = default;
	~CMonster() = default;

	void Initialize(Position pos, int type);
	void Update(CObject& my, const CObject& other);
	void Update();
	void Idle();
	void Attack(CPlayer& target);
	void Chase(const CPlayer& target);

	void UpdateWithClient();
	 
	void ChangeState(State* newState);

	bool IsindefPos();
	
	void SetPrevHealthPoint();

	void ResetRecoverCool();

	int TakeDamage(int atk_point);

	void Death();

	void UpdateTarget();

	// Getter & Setter
	void SetState(const int& state);
	void SetDefaultPos(const Position& pos);
	void SetActivityRange(const int& range);
	int GetHealthPoint() const;
	void SetHealthPoint(const int& hp);
	void SetChaseObj(const CObject& other);
	int GetState() const;
	void SetPosition(const Position& pos);
	Position GetPosition() const;
	Position GetDefPosition() const;
	Position GetRotation() const;
	int GetDistance(Position pos);
	void SetTarget(const int& t);
	int GetTarget() const;
	int GetID() const;
	int GetPrevHealthPoint() const;
	SC_OBJECT_ENTER MakeEnterPacket();
	SC_OBJECT_LEAVE MakeLeavePacket();
	SC_UPDATE_OBJ MakeUpdatePacket();
	SC_DAMAGED MakeDamagedPacket();
	void SetIndex(const int& idx);
	int GetEXP() const;
	int GetHP() const;

	void SetVelocity(const Position& v);
	void SetRotation(const Position& r);

	bool IsFront(const Position& player_pos);
};

float GetDegree(Position p1, Position p2);
void CreateMonsters(int num);
void SpawnMonster(int id, float x, float y, float z, int type);
void monster_thread();
void MonsterThread();

class State {
public:
	State() = default;
	~State() = default;

	virtual void Enter(CMonster*) = 0;
	virtual void Execute(CMonster*) = 0;
	virtual void Exit(CMonster*) = 0;
};

class IdleState : public State {
private:
	static IdleState* instance;

public:
	static IdleState* GetInstance();

	virtual void Enter(CMonster* mon);
	virtual void Execute(CMonster* mon);
	virtual void Exit(CMonster* mon);

};

class ChaseState : public State {
private:
	static ChaseState* instance;

public:
	static ChaseState* GetInstance();

	virtual void Enter(CMonster*);
	virtual void Execute(CMonster*);
	virtual void Exit(CMonster*);

};

class AttackState : public State {
private:
	static AttackState* instance;

public:
	static AttackState* GetInstance();

	virtual void Enter(CMonster*);
	virtual void Execute(CMonster*);
	virtual void Exit(CMonster*);

};

class ReturnHomeState : public State {
private:
	static ReturnHomeState* instance;

public:
	static ReturnHomeState* GetInstance();

	virtual void Enter(CMonster*);
	virtual void Execute(CMonster*);
	virtual void Exit(CMonster*);
};

class DeadState : public State {
private:
	static DeadState* instance;

public:
	static DeadState* GetInstance();

	virtual void Enter(CMonster*);
	virtual void Execute(CMonster*);
	virtual void Exit(CMonster*);

};