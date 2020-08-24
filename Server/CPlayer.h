#pragma once
#include <string>
#include <iostream>
#include <unordered_set>
#include <mutex>
#include "CObject.h"
#include "protocol.h"
#define MAX_PLAYER 10
#define MAX_CLIENTS 100
#define MAX_MOVE_RANGE 10
#define MAX_VIEW_RANGE 1000

enum Player_State {
	not_login = -1, In_Lobby, In_Room, In_Game,
	host, none_ready, play_game
};

class CPlayer {
private:
	std::string m_id;
	std::string m_pass;
	int state{not_login};
	int level;
	int exp;
	int m_roomNum;
	int m_idx;
	bool isReady{ false };
	bool m_host{ false };
	int healthPoint;
	int magicPoint;
	int atkPoint;
	int objType;
	Position pos;
	Position rotation;
	mutex pLock;
public:
	unordered_set<int> viewList;

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
	bool GetHost() { return m_host; }
	void SetIdx(int n) { m_idx = n; }
	int GetIdx() { return m_idx; }
	void SetPosition(const Position& pos);
	Position GetPosition() const;
	int GetDistance(Position pos);
	void SetRotation(const Position& rotation);

	// Packet
	SC_OBJECT_ENTER MakeEnterPacket();
	SC_UPDATE_OBJ MakeUpdatePacket();
	SC_OBJECT_LEAVE MakeLeavePacket();

	void EnterObj(int oid);
	void EnterGame();
	// ??
	void Ready();

	void Update(const CS_PLAYER& pack);
	void Update();

	void Attck();

};

