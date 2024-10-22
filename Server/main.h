#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib, "mswsock.lib")
#pragma comment (lib, "ws2_32.lib")

#include <WS2tcpip.h>
#include <vector>
#include <thread>
#include <map>
#include <mutex>
#include "globals.h"
#include "CDBConnector.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "KDTree.h"

enum ENUMOP { OP_RECV, OP_SEND, OP_ACCEPT, EV_MONSTER, EV_BOSS, EV_LOGIN, EV_SIGN, EV_UPDATE, EV_BERSERK, EV_ATK_OFF, EV_GUARD_OFF, EV_EVADE_OFF, EV_FIREBALL, EV_ASS_OFF
};

struct EXOVER {
	WSAOVERLAPPED over;
	ENUMOP op;
	char io_buf[BUFSIZE];
	union {
		WSABUF wsabuf;
		SOCKET c_sock;
	};
};

struct CLIENT {
	SOCKET sock;
	int id;
	EXOVER recv_over;
	int prev_size;
	char packet_buf[BUFSIZE];
	bool isconnected;
	mutex cLock;	// Client Lock
};


void recv_packet_construct(int uid, int io_byte);
void ProcessPacket(int uid, char* buf);
void send_packet(int uid, void* p);
void Disconnect(int uid);
void SignUp(const int& uid, const CS_LOGIN& pack);
void Login(const int& uid, const CS_LOGIN& pack);

// Thread Function
void worker_thread();
void TimerThread();
