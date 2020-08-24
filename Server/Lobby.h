#pragma once
#include "CPlayer.h"
#include <map>
#define NOT_JOINED -1

class Lobby {
private:
	int numUsers;
	int hostID;
	int state;
	int playerID[MAX_PLAYER];
public:
	Lobby();
	~Lobby() = default;

	int AddUser(const int& id);
	int SubUser();

	bool IsAllReady();
};

