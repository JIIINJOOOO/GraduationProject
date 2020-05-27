#pragma once
#include <string>
#include <iostream>
#include "CObject.h"
#include "packet.h"
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
	int m_state{not_login};
	int m_level;

	int m_roomNum;
	int m_idx;
	bool m_ready{ false };
	bool m_host{ false };

	Position pos;
public:
	CPlayer() = default;
	CPlayer(std::string id, std::string pass);
	~CPlayer();

	void Initialize(const CPlayer& p);

	// overloading
	bool operator== (const CPlayer& cp) const;

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
	// ??
	void Ready();

	void Update(const CS_PLAYER& pack);

	void Attck();

};

