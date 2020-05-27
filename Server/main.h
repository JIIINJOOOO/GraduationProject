#pragma once
#include "globals.h"
#include "CDBConnector.h"
#include "CPlayer.h"
#include "CMonster.h"
#include "KDTree.h"
#include "CServer.h"
enum ENUMOP { OP_RECV, OP_SEND, OP_ACCEPT };

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
};

void recv_packet_construct(int uid, int io_byte);
void ProcessPacket(int uid, char* buf);
void send_packet(int uid, void* p);
void Disconnect(int uid);
void SignUp(const int& uid, const CS_LOGIN& pack);
void Login(const int& uid, const CS_LOGIN& pack);

// Thread Function
void worker_thread();