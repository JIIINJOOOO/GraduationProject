/*
한국산업기술대학교 2020년 졸업작품 Last Hero 서버
2015182027 유재현
*/

// #include "CServer.h"

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment (lib, "ws2_32.lib")
#pragma comment (lib, "mswsock.lib")

#include <iostream>
#include <WS2tcpip.h>
#include <MSWSock.h>
#include <vector>
#include <thread>
#include <map>
#include "CTerrain.h"
#include "main.h"

// Global Values
SOCKET g_listenSocket;
HANDLE g_iocp;
CLIENT g_clients[MAX_PLAYER];
CDBConnector g_dbc;
CTerrain *g_tmap;
map<int, CPlayer*> g_player;
map<int, CMonster*> g_monster;
map<int, CObject*> g_obj;

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
	}

	CTerrain* tmap = new CTerrain;
	if (tmap->LoadMap("../Heightmap.r16")) 
		cout << "Success Loading Hightmap to Terrain!\n";
	else {
		cout << "Fail Loading Hightmap to Terrain!\n";
		exit(-1);
	}
	

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
	worker_threads.emplace_back(MonsterThread);
	for (auto& th : worker_threads) th.join();
}

void worker_thread() {
	while (true) {
		DWORD io_byte;
		ULONG_PTR key;
		WSAOVERLAPPED* over;
		GetQueuedCompletionStatus(g_iocp, &io_byte, &key, &over, INFINITE);
		EXOVER *exover = reinterpret_cast<EXOVER*>(over);
		int user_id = static_cast<int>(key);
		CLIENT& cl = g_clients[user_id];

		switch (exover->op) {
		case OP_RECV:
			if (io_byte == 0) Disconnect(user_id);
			else {
				recv_packet_construct(user_id, io_byte);
				ZeroMemory(&cl.recv_over, sizeof(cl.recv_over));
				DWORD flags = 0;
				WSARecv(cl.sock, &cl.recv_over.wsabuf, 1, NULL, &flags, &cl.recv_over.over, NULL);
			}
			break;
		case OP_SEND:
			if (io_byte == 0) Disconnect(user_id);
			delete exover;
			break;
		case OP_ACCEPT:{
			cout << "1234" << endl;
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
		}
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
	case login_packet:
	case signup_packet: {
		// Connect DB
		g_dbc.AllocateHandle(); 
		g_dbc.ConnectDataSource();
		
		CS_LOGIN* packet = reinterpret_cast<CS_LOGIN*>(buf);
		if (buf[1] == signup_packet) SignUp(uid, *packet);
		if (buf[1] == login_packet) Login (uid, *packet);

		g_dbc.DisconnectDataSource();
	}
		break;
	case move_packet: {
		CS_MOVE* pack = reinterpret_cast<CS_MOVE*>(buf);
		g_obj[uid]->SetPosition(pack->destination);
		g_player[uid]->SetPosition(pack->destination);

		SC_PLAYER_MOVE packet;
		packet.size = sizeof(SC_PLAYER_MOVE);
		packet.type = sc_player_move;
		packet.uid = uid;
		packet.pos = g_player[uid]->GetPosition();
		for (auto& cl : g_clients)
			if (cl.isconnected) {
				send_packet(cl.id, &packet);
			}
	}
		break;
	default:
		cout << "Unknown Packet Type Error!" << endl;
		DebugBreak();
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
	if (0 > uid || uid >= MAX_PLAYER) return;
	if (!g_clients[uid].isconnected) return;
	g_clients[uid].isconnected = false;
	g_player.erase(uid);

	SC_LEAVE pack;
	pack.size = sizeof(SC_LEAVE);
	pack.type = sc_leave;
	pack.id = uid;

	for (int i = 0; i < MAX_PLAYER; ++i)
		if (g_clients[i].isconnected)
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
		// g_obj[uid] = new CObject;
		g_player[uid]->Initialize(CPlayer(string(pack.id), string(pack.password)));
		// g_obj[uid]->Initialize(Position(), Velocity(), Volume(), Accel(), obj_player);
		for (int i = 0; i < MAX_PLAYER; ++i)
			if (g_clients[i].isconnected)
				send_packet(i, &packet);
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
				if (cl.isconnected)
					if (g_obj[cl.id]->GetDistance(g_obj[idx]->GetPosition()) < MAX_VIEW_RANGE)
						if (cl.isconnected) send_packet(cl.id, &packet);
		}
		

		Sleep(1000 / 60);
	}
}

