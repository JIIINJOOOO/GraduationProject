#pragma once
#include "CPlayer.h"
#include "packet.h"
#include "CObject.h"
#include "CMonster.h"
#define MAX_LOBBY 4	
#define MAX_PLAYER 4

#define IS_NOT_ROOM -1
#define None -1

#define DELETE_ROOM 1
#define HOST_CHANGE 2
#define SUB_SUCCESS 3
class CLobby {
private:
	int m_idx;
	int m_hostIdx;
	int m_state{ IS_NOT_ROOM };
	int m_users[MAX_PLAYER]{ None, None, None, None };
	int m_numOfUsers{ NULL };
	CObject *m_obj[MAX_OBJ];
	CMonster *m_monster[MAX_MONSTER];
public:
	CLobby();
	~CLobby();
	// CLobby() = default;

	int AddUser(const int& idx);
	void SubUsers(const CPlayer& player);
	int SubUsers(const int& idx);


	int GetUsers();
	void SetIdx(int idx);
	int GetIdx();
	void SetHost(const int& idx);
	std::string GetHostID();
	int GetState();
	int GetPlayerIdx(int n);
	bool Start();

	SC_PLAYER GetPlayer2Pack(int n);
	SC_LOBBY MakeLobbyPack(int state);

	void ReadyPlayer(int num);
	bool IsHost(const int& player);
	bool IsAllReady();

	void Update();

	void AddMonster(int n, Position pos, Velocity vel, Volume vol, Accel acc, int type);

	// Supervise Obj
	void SetObject(int playerIdx, int objIdx);
	int AddObject(Position pos, Velocity vel, Volume vol, Accel acc, int type);
	void DeleteObject(int idx);

	
};

