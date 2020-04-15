#pragma once
#include "CObject.h"
#include "CPlayer.h"
#include "CPathFinder.h"
#define MAX_MONSTER 1
#define ACTIVITY_RANGE 4000
#define ATTACK_RANGE 100
#define CHASE_RANGE 1000
#define MONSTER_MAX_HP 100
#define BOSS_IDX 1000
#define START_POINT_MONSTER 100
#define IDLE_RANGE 5
enum monster_state {
	idle, attack, move, chase, return_home,
	normal, boss
};
class CMonster
{
private:
	int m_myIdx;
	int xIdx, zIdx;
	
	Position m_defaultPos;
	CPathFinder *m_pathFinder;
	list<Position> m_chasePath;
	float z_range{ 30.f };
	// CObject &m_obj;
	int m_activityRange;
	int m_state{idle};
	int m_healthPoint;
	int m_type;
public:
	CMonster() = default;
	~CMonster() = default;

	void Initialize(Position pos, int type);
	void Update(CObject& my, const CObject& other);
	void Idle();
	void Attack();
	void Chase(const CPlayer& target);

	// Getter & Setter
	void SetState(const int& state);
	void SetDefaultPos(const Position& pos);
	void SetActivityRange(const int& range);
	int GetHealthPoint() const;
	void SetChaseObj(const CObject& other);
	int GetState() const;
};

