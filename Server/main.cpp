/*
한국산업기술대학교 2020년 졸업작품 Last Hero 서버
2015182027 유재현
*/

#include "main.h"
#include <iostream>
#include <mutex>
#include <MSWSock.h>
#include "CTerrain.h"
#include "Event.h"
#include "Boss.h"

// Global Values
SOCKET g_listenSocket;
HANDLE g_iocp;
CLIENT g_clients[MAX_PLAYER];
CDBConnector g_dbc;
CTerrain *g_tmap;
map<int, CPlayer*> g_player;
map<int, CMonster*> g_monster;
map<int, CObject*> g_obj;
Boss *boss;
priority_queue<int> timerQueue;
extern queue<DB_EVENT> quaryQueue;

bool mons[4];

int main() {
	int retval;
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return 0;
	g_listenSocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (g_listenSocket == INVALID_SOCKET) return 0; // error quit
	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(SERVERPORT);
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	::bind(g_listenSocket, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr));

	listen(g_listenSocket, SOMAXCONN);
	g_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);

	// 초기화
	for (int i = 0; i < MAX_PLAYER; ++i) {
		g_clients[i].isconnected = false;
		g_clients[i].id = i;
		// g_player[i] = new CPlayer;
	}

	CTerrain* tmap = new CTerrain;
	if (tmap->LoadMap("../Heightmap.r16")) 
		cout << "Success Loading Hightmap to Terrain!\n";
	else {
		cout << "Fail Loading Hightmap to Terrain!\n";
		exit(-1);
	}
	CreateMonster(MAX_MONSTER);

	CreateIoCompletionPort(reinterpret_cast<HANDLE>(g_listenSocket), g_iocp, 999, 9);
	SOCKET cs = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	EXOVER accept_over;
	ZeroMemory(&accept_over.over, sizeof(accept_over.over));
	accept_over.op = OP_ACCEPT;
	accept_over.c_sock = cs;
	AcceptEx(g_listenSocket, cs, accept_over.io_buf, NULL, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &accept_over.over);
	
	vector<thread> worker_threads;
	for (int i = 0; i < 6; ++i)
		worker_threads.emplace_back(worker_thread);
	worker_threads.emplace_back(TimerThread);
	worker_threads.emplace_back(DB_Thread);
	for (auto& th : worker_threads) th.join();
}

void worker_thread() {
	while (true) {
		DWORD io_byte;
		ULONG_PTR key;
		WSAOVERLAPPED* over;
		GetQueuedCompletionStatus(g_iocp, &io_byte, &key, &over, INFINITE);
		EXOVER* exover = reinterpret_cast<EXOVER*>(over);
		int user_id = static_cast<int>(key);
		CLIENT& cl = g_clients[user_id];

		switch (exover->op) {
		case OP_RECV:
			if (io_byte == 0) Disconnect(user_id);
			else {
				recv_packet_construct(user_id, io_byte);
				ZeroMemory(&cl.recv_over.over, sizeof(cl.recv_over.over));
				DWORD flags = 0;
				WSARecv(cl.sock, &cl.recv_over.wsabuf, 1, NULL, &flags, &cl.recv_over.over, NULL);
			}
			break;
		case OP_SEND:
			if (io_byte == 0) Disconnect(user_id);
			delete exover;
			break;
		case OP_ACCEPT:{
			cout << "New Client Connect \n";
			int user_id = -1;
			for (int i = 0; i < MAX_PLAYER; ++i)
				if (!g_clients[i].isconnected) {
					user_id = i;
					break;
				}
			SOCKET cs = exover->c_sock;
			if (user_id == -1) closesocket(cs);
			else {
				CreateIoCompletionPort(reinterpret_cast<HANDLE>(cs), g_iocp, user_id, 0);
				CLIENT& nc = g_clients[user_id];
				nc.id = user_id;
				nc.prev_size = 0;
				nc.recv_over.op = OP_RECV;
				g_player[user_id] = new CPlayer;
				g_player[user_id]->SetIdx(user_id);

				ZeroMemory(&nc.recv_over.over, sizeof(nc.recv_over.over));
				nc.recv_over.wsabuf.buf = nc.recv_over.io_buf;
				nc.recv_over.wsabuf.len = BUFSIZE;
				nc.sock = cs;
				nc.isconnected = true;
				DWORD flags = 0;
				WSARecv(cs, &nc.recv_over.wsabuf, 1, NULL, &flags, &nc.recv_over.over, NULL);
			}
			cs = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
			exover->c_sock = cs;
			ZeroMemory(&exover->over, sizeof(exover->over));
			AcceptEx(g_listenSocket, cs, exover->io_buf, NULL, sizeof(sockaddr_in) + 16, sizeof(sockaddr_in) + 16, NULL, &exover->over);
		}break;

		case EV_BOSS:
			boss->Update();
			delete exover;
			break;
		case EV_MONSTER:
			// if (user_id < NPC_ID_START) break;
			// if (g_monster[user_id] != NULL)
			// 	g_monster[user_id]->Update();
			delete exover;
			break;
		case EV_FIREBALL:
			g_player[user_id]->fireball.Update();
			delete exover;
			break;
		}
	}
}
void recv_packet_construct(int uid, int io_byte) {
	CLIENT& cu = g_clients[uid];
	EXOVER& ro = cu.recv_over;
	int rest_byte = io_byte;
	char* p = ro.io_buf;
	int packet_size = 0;
	if (cu.prev_size != 0) packet_size = cu.packet_buf[0];
	while (rest_byte > 0) {
		if (packet_size == 0) packet_size = p[0];
		if (packet_size <= rest_byte + cu.prev_size) {
			memcpy(cu.packet_buf + cu.prev_size, p, packet_size - cu.prev_size);
			p += packet_size - cu.prev_size;
			rest_byte -= packet_size - cu.prev_size;
			packet_size = 0;
			ProcessPacket(uid, cu.packet_buf);
			cu.prev_size = 0;
		}
		else {
			memcpy(cu.packet_buf + cu.prev_size, p, rest_byte);
			cu.prev_size += rest_byte;
			rest_byte = 0;
			p += rest_byte;
		}
	}
}

void ProcessPacket(int uid, char* buf) {
	switch (buf[1]) {
	case cs_login:
	case cs_signup: {
		// Connect DB
		//g_dbc.AllocateHandle(); 
		//g_dbc.ConnectDataSource();
		
		CS_LOGIN* packet = reinterpret_cast<CS_LOGIN*>(buf);
		if (buf[1] == cs_signup) SignUp(uid, *packet);
		if (buf[1] == cs_login) {
			Login(uid, *packet);
			// SC_LOGIN_OK pack;
			// pack.uid = uid;
			// pack.type = sc_login_ok;
			// pack.size = sizeof(SC_LOGIN_OK);
			// strcpy_s(pack.name, g_player[uid]->GetID().c_str());	
			// send_packet(uid, &pack);
			g_player[uid]->EnterGame();
		}
		//g_dbc.DisconnectDataSource();
	}
		break;
	case move_packet: {
		CS_MOVE* pack = reinterpret_cast<CS_MOVE*>(buf);
		if (g_player[uid] == NULL) break;
		g_player[uid]->SetVelocity(pack->velocity);
		g_player[uid]->SetRotation(pack->rotation);
		g_player[uid]->MoveTo(pack->destination);
		// 좌표 관련 send도 함수 안에서 이루어짐
	}break;
	case cs_chat: {
		CS_CHAT* pack = reinterpret_cast<CS_CHAT*>(buf);
		SC_CHAT s_pack;
		s_pack.size = sizeof(SC_CHAT);
		s_pack.type = sc_chat;
	}break;
	case cs_attack: {
		g_player[uid]->Attck();
	}break;
	case cs_fireball: {
		SC_FIREBALL pack;
		pack.size = sizeof(SC_FIREBALL);
		pack.type = sc_fireball;
		pack.oid = uid;
		g_player[uid]->FireBall();
		for (auto& oid : g_player[uid]->viewList)
			if (oid <= MAX_PLAYER)
				send_packet(oid, &pack);
	}break;
	case cs_guard: {
		SC_OBJ_GUARD pack;
		pack.size = sizeof(SC_OBJ_GUARD);
		pack.type = sc_guard;
		pack.oid = uid;
		// 아ㅓㅁ너램ㄹ너ㅐ 한시간을 고민했는데 호출을 안한거면 어쩌자ㅣ는거영창ㄹ ㅅㅂ
		g_player[uid]->Guard();
		for (auto& oid : g_player[uid]->viewList)
			if (oid < MAX_PLAYER)
				send_packet(oid, &pack);
	}break;
	case cs_jump: {
		SC_JUMP pack;
		pack.size = sizeof(SC_JUMP);
		pack.type = sc_jump;
		pack.oid = uid;
		for (auto& oid : g_player[uid]->viewList)
			if (oid < MAX_PLAYER)
				send_packet(oid, &pack);
	}break;
	case cs_evade: {
		SC_EVADE pack;
		pack.size = sizeof(SC_EVADE);
		pack.type = sc_evade;
		pack.oid = uid;
		g_player[uid]->Evade();
		for (auto& oid : g_player[uid]->viewList)
			if (oid < MAX_PLAYER)
				send_packet(oid, &pack);
	}break;
	case cs_weapon_on: {
		if (g_player[uid]->isWpnOn) break;
		SC_WEAPON_ON pack;
		pack.size = sizeof(SC_WEAPON_ON);
		pack.type = sc_weapon_on;
		pack.oid = uid;
		g_player[uid]->isWpnOn = true;
		for (auto& oid : g_player[uid]->viewList)
			if (oid < MAX_PLAYER)
				send_packet(oid, &pack);
	}break;
	case cs_weapon_off: {
		SC_WEAPON_OFF pack;
		pack.size = sizeof(SC_WEAPON_OFF);
		pack.type = sc_weapon_off;
		pack.oid = uid;
		g_player[uid]->isWpnOn = false;
		for (auto& oid : g_player[uid]->viewList)
			if (oid < MAX_PLAYER)
				send_packet(oid, &pack);
	}break;
	case cs_sword_on: {
		g_player[uid]->WeaponOn(wpn_sword);
		
	}break;
	case cs_sword_off: {
		g_player[uid]->WeaponOff();
		
	}break;
	case cs_hammer_on: {
		g_player[uid]->WeaponOn(wpn_hammer);
		
	}break;
	case cs_hammer_off: {
		g_player[uid]->WeaponOff();
		
	}break;
	case cs_berserk: {
		g_player[uid]->Berserk();
	}break;
	case cs_ready: {
		g_player[uid]->Ready();
	}break;
	case cs_boss_move: {
		CS_BOSS_MOVE* pack = reinterpret_cast<CS_BOSS_MOVE*>(buf);
		boss->SetPosition(pack->destination);
		boss->SetRotation(pack->rotation);
	}break;
	case cs_hide: {
		g_player[uid]->Hide();
	}break;
	case cs_hide_off: {
		g_player[uid]->HideOff();
	}break;
	case cs_assassin: {
		g_player[uid]->Assassinate();
	}break;
	case cs_move_stop: {
		SC_MOVE_STOP pack{ sizeof(SC_MOVE_STOP), sc_move_stop, uid };
		for (int i = 0; i < MAX_PLAYER; ++i) {
			if (g_player[i] == NULL) continue;
			send_packet(i, &pack);
		}
	}break;
	case cs_npc_move: {
		CS_NPC_MOVE* p = reinterpret_cast<CS_NPC_MOVE*>(buf);
		SC_UPDATE_OBJ pack{ sizeof(pack), sc_update_obj, p->pos, p->oid, p->roatation, p->velocity };
		return;
		/*if (p->oid == BOSS_IDX) {
			cout << "asd" << endl;
			for (int i = 0; i < MAX_PLAYER; ++i) {
				if (g_player[i] == NULL) continue;
				if (g_player[i]->isHost) continue;
				if (uid == i) continue;
				send_packet(i, &pack);
			} 
			return;
		}*/
		if (g_monster[p->oid] == NULL) return;
		g_monster[p->oid]->SetPosition(p->pos);
		g_monster[p->oid]->SetVelocity(p->velocity);
		g_monster[p->oid]->SetRotation(p->roatation);
		g_monster[p->oid]->Update();
		// g_monster[p->oid]->Update();
		// g_monster[p->oid].set
		// cout << pack.oid << endl;
		// cout << uid << "\t" << p->oid << endl;
		// for (int i = 0; i < MAX_PLAYER; ++i) {
		// 	if (g_player[i] == NULL) continue;
		// 	if (g_player[i]->isHost) continue;
		// 	if (uid == i) continue;
		// 	send_packet(i, &pack);
		// }
	}break;
	case cs_spawn_npc: {
		CS_SPAWN_NPC* pack = reinterpret_cast<CS_SPAWN_NPC*>(buf);
		cout << "몬스터(id=" << pack->oid << ") 스폰(" << pack->pos.x << ", " << pack->pos.y << ", " << pack->pos.z << ")" << endl;
		if (g_monster[pack->oid] == NULL) g_monster[pack->oid] = new CMonster;
		g_monster[pack->oid]->Initialize(pack->pos, normal);
		g_monster[pack->oid]->SetIndex(pack->oid);
	}break;
	default:
		cout << "Unknown Packet Type Error!" << endl;
		Disconnect(uid);
		// DebugBreak();
		exit(-1);
	}
}

void send_packet(int uid, void* p) {
	char* buf = reinterpret_cast<char*>(p);
	CLIENT& c = g_clients[uid];
	EXOVER* exover = new EXOVER;
	exover->op = OP_SEND;
	ZeroMemory(&exover->over, sizeof(exover->over));
	exover->wsabuf.buf = exover->io_buf;
	exover->wsabuf.len = buf[0];
	memcpy(exover->io_buf, buf, buf[0]);
	WSASend(c.sock, &exover->wsabuf, 1, NULL, 0, &exover->over, NULL);
}

void Disconnect(int uid) {
	cout << "Disconnect " << g_player[uid]->GetID() << endl;
	if (0 > uid || uid >= MAX_PLAYER) return;
	g_player[uid]->isAlive = false;
	// g_player.erase(uid);
	g_player[uid] = NULL;
	g_clients[uid].isconnected = false;
	closesocket(g_clients[uid].sock);

	SC_LEAVE pack;
	pack.size = sizeof(SC_LEAVE);
	pack.type = sc_leave;
	pack.id = uid;

	for (int i = 0; i < MAX_PLAYER; ++i)
		if (g_player[i] != NULL)
			send_packet(i, &pack);
}

void SignUp(const int& uid, const CS_LOGIN& pack) {
	string sql = "INSERT INTO Account VALUES (\'" + (string)pack.id +
		"\', \'" + (string)pack.password + "\')";
	int ret = g_dbc.ExcuteStatementDirect((SQLCHAR*)sql.c_str());
	if (ret == SQL_EXUTE_OK) {
		cout << "Create New Account - ID : " << pack.id << endl;
		SC_SIGNUP_OK packet;
		packet.size = sizeof(SC_SIGNUP_OK);
		packet.type = sc_signup_ok;
		send_packet(uid, &packet);
	}
	else {
		cout << "Create Account Fail - ID : " << pack.id << " (Duplicated ID or SQL Error)\n";
		SC_SIGNUP_FAIL packet;
		packet.size = sizeof(SC_SIGNUP_FAIL);
		packet.type = sc_signup_fail;
		send_packet(uid, &packet);
	}
}

void Login(const int& uid, const CS_LOGIN& pack) {
	string sql = "select * from Account where id  = \'" + (string)pack.id +
		"\' and password = \'" + (string)pack.password + "\'";
	AddQuary(uid, 5, pack.id, pack.password);
	return;
	int status;	// check login status (fail, success)
	int ret = g_dbc.ExcuteStatementDirect((SQLCHAR*)sql.c_str());
	if (g_dbc.RetrieveResult(pack.id, pack.password)) status = login_ok;
	else status = login_error;
	
	if (ret == SQL_EXUTE_OK && status == login_ok) {
		cout << pack.id << " is Log-in Success\n";
		SC_LOGIN_OK packet;
		packet.size = sizeof(SC_LOGIN_OK);
		packet.type = sc_login_ok;

		g_player[uid] = new CPlayer;
		auto& my = g_player[uid];
		my->Initialize(CPlayer(string(pack.id), string(pack.password)));
		my->SetState(In_Game);
		my->SetIdx(uid);
		cout << "send_login_ok" << endl;
		send_packet(uid, &packet);	// login_ok

		// send enter
		SC_OBJECT_ENTER to_other_pack;
		strcpy_s(to_other_pack.name, pack.id);
		to_other_pack.o_type = 0;	// 모델링 타입
		to_other_pack.pos = my->GetPosition();
		to_other_pack.size = sizeof(SC_OBJECT_ENTER);
		to_other_pack.type = sc_enter_obj;

		for (auto& p : g_player) {
			if (p.second == NULL) continue;
			if (my->GetDistance(p.second->GetPosition()) < MAX_VIEW_RANGE) {
				send_packet(p.second->GetIdx(), &to_other_pack);
			}
		}
		return;
	}
	else {
		cout << pack.id << " is Log-in Fail (Invalid ID or Password)\n";
		SC_LOGIN_FAIL packet;
		packet.size = sizeof(SC_LOGIN_FAIL);
		packet.type = sc_login_fail;
		send_packet(uid, &packet);
	}
}

void monster_thread() {
	CreateMonster(MAX_MONSTER);
	while (true) {
		for (int i = 0; i < MAX_MONSTER; ++i) {
			short idx = START_POINT_MONSTER + i;
			if (g_monster[idx] == NULL) continue;
			if (g_monster[idx]->GetHealthPoint() < 0) continue;
			int distance;
			int firstNearPlayer = 0;
			bool isdetected = false;

			// 시야 내의 가장 가까운 플레이어 탐색
			for (int j = 0; j < MAX_PLAYER; ++j) {
				if (g_player[j] == NULL) continue;
				isdetected = true;
				distance = g_obj[idx]->GetDistance(g_obj[j]->GetPosition());
				if (distance > g_obj[idx]->GetDistance(g_obj[firstNearPlayer]->GetPosition())) continue;
				if (distance < CHASE_RANGE) firstNearPlayer = j;
			}

			if (!isdetected) continue;

			// 가장 가까운 플레이어
			distance = g_obj[idx]->GetDistance(g_obj[firstNearPlayer]->GetPosition());
			if (distance < ATTACK_RANGE) g_monster[idx]->SetState(attack);
			else if (distance < CHASE_RANGE) g_monster[idx]->SetState(chase);
			else if (g_obj[idx]->GetDistance(g_obj[idx]->GetPosition()))
				g_monster[idx]->SetState(return_home);

			SC_UPDATE_OBJ packet;
			packet.size = sizeof(SC_UPDATE_OBJ);
			packet.type = sc_update_obj;
			packet.pos = g_obj[idx]->GetPosition();

			for (auto& cl : g_clients)
				if (cl.isconnected) send_packet(cl.id, &packet);
		}
		

		Sleep(1000 / 60);
	}
}

