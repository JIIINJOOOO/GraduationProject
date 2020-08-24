#include "Lobby.h"
extern map<int, CPlayer*> g_player;

Lobby::Lobby(){
	numUsers = 0;
	hostID = -1;
	for (auto& id : playerID)
		id = NOT_JOINED;
}

int Lobby::AddUser(const int& id) {
	for (auto& pid : playerID)
		if (pid == NOT_JOINED) pid = id;
	numUsers++;
}

bool Lobby::IsAllReady() {
	for (auto& id : playerID)
		if (id == NOT_JOINED) continue;
		else if (g_player[id]->GetState() != ready)
			return false;
	return true;
}