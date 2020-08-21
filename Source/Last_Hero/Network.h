// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#pragma comment(lib, "ws2_32")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "Windows/PreWindowsApi.h" // winnt.h(536): warning C4005: 'TEXT': macro redefinition
#include <WS2tcpip.h>
#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <queue>
#include "protocol.h"

#define MAXLEN 10
#define SERVERIP "192.168.10.30"
#define SERVERPORT 9000
#define BUFSIZE 512
#define MAX_USERS 4
#define NPC_ID_START 10000
#define MAX_MONSTER 100
/**
 *
 */
using namespace std;


class LAST_HERO_API Network {
struct GMB_Event {
	// 받은 패킷 정보를 이벤트를 통해 넘겨준다?
	char type;
	short oid;
	Position pos;
	short hp;
	short anim_id;
	Position rotation;
};

private:
	HANDLE m_sendEvent;
	char sendBuf[BUFSIZE];
	P_STATE m_status;
	string id, pass;
	bool isHost;
	int my_id;
public:
	SOCKET m_sock;
	char recvBuf[BUFSIZE];
	mutex gmbLock;
	queue<GMB_Event> eventQue;
	mutex eventLock;

	HANDLE loginEvent;

public:
	Network();
	~Network() = default;

	void SetAccountInfo(const char* id, const char* pass);
	void ProcessPacket(char* buf);

	void SendPacket(void* packet);

	// static void RecvThread(void* net);
	static DWORD WINAPI RecvThread(LPVOID p);
	int GetStatus() const;
	int GetMyID() const;
};

int recvn(SOCKET s, char *buf, int len, int flags);