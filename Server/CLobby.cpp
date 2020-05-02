#include "CLobby.h"
#include "globals.h"

CPlayer* g_players[MAX_CLIENTS];

CLobby::CLobby() {
	for (int i = 0; i < MAX_OBJ; ++i)
		m_obj[i] = NULL;
}
CLobby::~CLobby() {
	for (auto obj : m_obj) {
		delete obj;
		obj = NULL;
	}

}
int CLobby::AddUser(const int& idx) {
	int Useridx{ -1 };
	if (idx < 0 || idx >= MAX_PLAYER) return ADD_FAIL;
	if (m_numOfUsers >= MAX_PLAYER) return ADD_FAIL;
	
	// 대충 게임중이면 참가 못한다는 코드
	for (int i = 0; i < MAX_PLAYER; ++i)
		if (m_users[i] == None) {
			// i = idx;
			m_users[i] = idx;
			Useridx = i;
			break;
		}
	if (Useridx == ADD_FAIL) return ADD_FAIL;
	printf("%s is Join - %d Room \n", g_players[Useridx]->GetID().c_str(), m_idx);
	++m_numOfUsers;
	// 여기서 플레이어한테 방 번호를 넣어주는게 좋으려나????
	printf("num - %d \n", m_numOfUsers);
	return Useridx;
	/*
	지금 여기서 문제가 m_player를 하나로 만들면서
	여기서 액세스를 못하게 되버렸어
	전역으로 빼버리는게 맞나?
	아직은 문제없네?

	나중에 여기서 액세스할 일이 생기려나?

	*/
}
int CLobby::SubUsers(const int& idx) {
	if (idx < 0 || idx >= MAX_CLIENTS) return ADD_FAIL;

	printf("%s is leave Game - %d\n", g_players[idx]->GetID().c_str(), m_idx);
	m_users[idx] = None;
	--m_numOfUsers;
	if (m_numOfUsers <= 0 && m_hostIdx == idx) {
		// If no user, delete room
		printf("%d room is delete (no Users)\n", m_idx);
		m_idx = IS_NOT_ROOM;
		m_state = IS_NOT_ROOM;
		return DELETE_ROOM;
	}
	else if (m_hostIdx == idx) {
		for (int i = 0; i < MAX_PLAYER; ++i)
			if (m_users[i] != None) {
				printf("%d room host is changed - %s to %s\n", m_idx, 
					g_players[m_hostIdx]->GetID().c_str(), g_players[i]->GetID().c_str());
				m_hostIdx = i;
				return HOST_CHANGE;
			}
	}
	return SUB_SUCCESS;
}

void CLobby::SubUsers(const CPlayer& player) {
	//for (int i = 0; i < MAX_PLAYER; ++i) {
	//	if (m_player[i] == player) {
	//		m_player[i].OutLobby();
	//
	//		if (i == m_hostIdx)
	//			// If no user without host, delete room 
	//			if (m_users <= 1) {
	//				printf("%d room is delete (no Users)\n", m_idx);
	//				m_idx = IS_NOT_ROOM;
	//				m_state = IS_NOT_ROOM;
	//			}
	//			// If another user, host change 
	//			else 
	//				for (int j = 0; j < MAX_PLAYER; ++j)
	//					if (m_player[j].GetState() != not_login) {
	//						m_hostIdx = j;
	//						printf("%d room host is changed - %s\n", m_idx, m_player[m_hostIdx].GetID//().c_str());
	//						if (m_state == full) m_state = standby;
	//						break;
	//					}
	//	}
	//}
	//--m_users;
}
SC_PLAYER CLobby::GetPlayer2Pack(int n) {
	// m_player 's index : n (0~3)을 입력받아 해당하는 플레이어의 패킷을 만들어준다.
	SC_PLAYER player;
	player.type = player_packet;
	// if (m_player[n].GetState() == not_login) return SC_PLAYER();
	// player.idx = n;
	// if (n == m_hostIdx) player.state = host;
	// else if (m_player[n].GetState() == ready) player.state = ready;
	// else if (m_player[n].GetState() == start) player.state = start;
	// else player.state = none_ready;
	// strcpy_s(player.id, m_player[n].GetID().c_str());
	// m_player[n].ShowPlayerInfo();
	return player;
}
void CLobby::ReadyPlayer(int num) {
	if (num == m_hostIdx) return;
	g_players[num]->Ready();
}
bool CLobby::IsHost(const int& player) {
	return m_hostIdx == player;
}
bool CLobby::IsAllReady() {
	for (int i = 0; i < MAX_PLAYER; ++i)
		if (m_users[i] != None) {
			int state = g_players[m_users[i]]->GetState();
			if (state == not_login || i == m_hostIdx) continue;
			else if (state != ready) return false;
		}
	return true;
}
bool CLobby::Start() {
	if (IsAllReady() == false) {
		printf("%d room is start fail - host:%s\n", m_idx, g_players[m_hostIdx]->GetID().c_str());
		return false;
	}
	
	for (int i = 0; i < MAX_PLAYER; ++i)
		if (m_users[i] != None)
			g_players[m_users[i]]->SetState(start);
	m_state = ingame;
	printf("%d room is start ok - host:%s\n", m_idx, g_players[m_hostIdx]->GetID().c_str());
	for (int i = 0; i < MAX_MONSTER; ++i)
		AddMonster(i, Position(), Velocity(), Volume(), Accel(), obj_monster);
	return true;
}
void CLobby::SetObject(int playerIdx, int objIdx) {
	// m_player[playerIdx].SetObjIdx(objIdx);
	// g_players[m_users[playerIdx]]->SetObjIdx(objIdx);
}
SC_LOBBY CLobby::MakeLobbyPack(int state) {
	SC_LOBBY scLobby;
	scLobby.idx = m_idx;
	scLobby.users = m_numOfUsers;
	scLobby.state = state;
	scLobby.type = lobby_packet;
	return scLobby;
}
int CLobby::AddObject(Position pos, Velocity vel, Volume vol, Accel acc, int type) {
	int idx{ -1 };
	for (int i = 0; i < MAX_OBJ; ++i)
		if (m_obj[i] == NULL) {
			printf("!@34");
			idx = i;
			break;
		}
	if (idx == ADD_FAIL) {
		printf("Can't Add Object \n");
		return ADD_FAIL;
	}
	m_obj[idx] = new CObject;
	m_obj[idx]->Initialize(pos, vel, vol, acc, type);
	return idx;
}
void CLobby::DeleteObject(int idx) {
	if (idx < 0 || idx >= MAX_OBJ) {
		printf("Delete Obj Error - Index is Exceed Max or Negative \n");
		return;
	}
	if (m_obj[idx] == NULL) return;
	delete m_obj[idx];
	m_obj[idx] = NULL;
}
void CLobby::AddMonster(int n, Position pos, Velocity vel, Volume vol, Accel acc, int type) {
	/*
	0~n 번의 몬스터를 생성해야한다.
	위치 등 물리값들을 미리 지정해준다
	오브젝트도 추가해준다

	*/
	m_monster[n] = new CMonster;
	int objIdx = AddObject(pos, vel, vol, acc, type);
	if (objIdx == ADD_FAIL) return;
	m_monster[n]->SetObjIdx(objIdx);
	m_monster[n]->SetDefaultPos(pos);
	m_monster[n]->SetActivityRange(ACTIVITY_RANGE);
	printf("Create Monster in %d Room (Monster Index - %d, %d)\n", m_idx, n, objIdx);
}
void CLobby::Update() {
	// Delete expired Object
//	for (auto obj : m_obj)
//		if (obj == NULL) continue;
//		else if (obj->GetDeleteCoolTime < 0) {
//			delete obj;
//			obj = NULL;
//		}
	// Delete dead Monster
	for (auto monster : m_monster)
		if (monster == NULL) continue;
		else if (monster->GetHealthPoint() <= 0) {
			delete monster;
			monster = NULL;
		}
		// else monster->Update(m_users);
	
}
// Getter & Setter
int CLobby::GetUsers() {
	return m_numOfUsers;
}
void CLobby::SetIdx(int idx) {
	m_idx = idx;
}
int CLobby::GetIdx() {
	return m_idx;
}
void CLobby::SetHost(const int& idx) {
	for (int i = 0; i < MAX_PLAYER; ++i)
		if (m_users[i] == idx) {
			m_hostIdx = i;
			break;
		}
}
std::string CLobby::GetHostID() {
	return g_players[m_hostIdx]->GetID();
}
int CLobby::GetState() {
	return m_state;
}
int CLobby::GetPlayerIdx(int n) {
	return m_users[n];
}