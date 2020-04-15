#pragma once
#include "CObject.h"
#include "CPlayer.h"
#define MAX_MONSTER 5
#define ACTIVITY_RANGE 100
#define ATTACK_RANGE 10
#define MONSTER_MAX_HP 100
#define BOSS_IDX 1000
#define START_POINT_MONSTER 100
enum monster_state {
	idle, attack, move, chase,
	normal, boss
};
class CMonster
{
private:
	int m_myIdx;

	int xIdx, zIdx;
	
	Position m_defaultPos;
	int m_activityRange;
	int m_state;
	int m_healthPoint;
	int m_type;
public:
	CMonster() = default;
	~CMonster() = default;

	void Initialize(Position pos, int type);

	void Update(const CObject& other);
	void Idle();
	void Attack();
	void Chase(const CPlayer& target);

	// Getter & Setter
	void SetObjIdx(const int& idx);
	void SetDefaultPos(const Position& pos);
	void SetActivityRange(const int& range);
	int GetHealthPoint() const;
	Position GetPosition() const;
};

