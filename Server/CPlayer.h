#pragma once
#include <string>
#include <iostream>
#include <unordered_set>
#include <mutex>
#include <chrono>
#include "CObject.h"
#include "protocol.h"
#define MAX_PLAYER 10
#define MAX_CLIENTS 100
#define MAX_MOVE_RANGE 10
#define MAX_VIEW_RANGE 5000
#define BERSERK_RANGE 2700
#define BERSERK_BONUS 2
#define LEVEL_UP_BONUS 10
#define GUARD_RANGE 200

#define MAX_SWORD_COMBO 3

enum Player_State {
	not_login = -1, In_Lobby, In_Room, In_Game,
	host, none_ready, play_game
};
using namespace std::chrono;

class CPlayer {
private:
	std::string name;
	std::string m_pass;
	int state{not_login};
	int level;
	int exp;
	int id;
	bool isReady{ false };
	int healthPoint;
	int MaxHP;
	int magicPoint;
	int MaxMP;
	int atkPoint;
	int objType;
	Position pos;
	Position rotation;
	Position velocity;
	mutex pLock;
	bool isBerserk;

	Weapon_Type wpnType;
public:
	CObject fireball;
	unordered_set<int> viewList;
	bool isWpnOn;
	bool isGuard;
	bool isAttack;
	bool isAlive;
	bool isEvade;
	bool isSpellFireball;
	bool isHide;
	bool isAssasinate;
	bool isHost;

	int comboCnt;
	high_resolution_clock::time_point lastAtkTime;	// ÄÞº¸°ø°Ý
public:
	CPlayer() = default;
	CPlayer(std::string id, std::string pass);
	~CPlayer();

	void Initialize(const CPlayer& p);
	void Initialize(int hp, int o_type, int exp, int lv, int mp, int atk, int x, int y);
	// overloading
	bool operator== (const CPlayer& cp) const;
	
	void MoveTo(const Position& p);

	// Getter & Setter
	void SetState(const int& state);
	int GetState();
	std::string GetID();
	int GetRoomNum();
	void SetIdx(int n) { id = n; }
	int GetIdx() { return id; }
	void SetPosition(const Position& pos);
	Position GetPosition() const;
	int GetDistance(Position pos);
	void SetRotation(const Position& rotation);
	void SetName(const char* name);
	void SetPass(const char* pass);
	void SetVelocity(const Position& v);

	int GetMaxHP() const;
	int GetMaxMP() const;
	int GetEXP() const;
	int GetLevel() const;

	// Packet
	SC_OBJECT_ENTER MakeEnterPacket();
	SC_UPDATE_OBJ MakeUpdatePacket();
	SC_OBJECT_LEAVE MakeLeavePacket();
	SC_GET_EXP MakeGetExpPacket();

	void EnterObj(int oid);
	void EnterGame();
	// ??
	void Ready();

	void Update(const CS_PLAYER& pack);
	void Update();

	void Attck();
	void Berserk();
	void BerserkOff();
	void KillMonster(int oid);
	void LevelUp();
	void Guard();
	void TakeDamage(int damage);
	void Evade();
	void FireBall();
	void Hide();
	void HideOff();
	void Assassinate();
	void WeaponOn(const Weapon_Type& wpn);
	void WeaponOff();
};

