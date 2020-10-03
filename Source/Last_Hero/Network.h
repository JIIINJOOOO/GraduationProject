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
#include <queue>
#include <unordered_map>
#include <chrono>
#undef UpdateResource
#undef PlaySound
#include "protocol.h"


#define MAXLEN 10
#define SERVERIP "1.11.57.96"
#define SERVERPORT 9000
#define BUFSIZE 512
#define MAX_USERS 10
#define NPC_ID_START 10000
#define MAX_MONSTER 100
#define GMB_ID 999
/**
 *
 */
using namespace std;
using namespace chrono;

class LAST_HERO_API Network {
public:
	struct GMB_Event {
		// ���� ��Ŷ ������ �̺�Ʈ�� ���� �Ѱ��ش�?
		char type;
		short oid;
		Position pos;
		short hp;
		short mp;	// ���ݽ� �޺� ī��Ʈ �뵵�ε� ���
		short level;
		short exp;
		char o_type;
		Position rotation;
		Position velocity;


	};

private:
	HANDLE m_sendEvent;
	char sendBuf[BUFSIZE];
	P_STATE m_status;
	string id, pass;
	int my_id;
public:
	bool isHost;
	SOCKET m_sock;
	char recvBuf[BUFSIZE];
	queue<GMB_Event> eventQue;
	// Test eventLock;
	unordered_map<int, queue<GMB_Event>> objEventQue;

	HANDLE loginEvent;
	Weapon_Type wpnType{ wpn_none };
	bool isMovingN;

	int gob_target{ -100 };
	Position my_pos;
	Position my_rot;
	Position my_vel;
	high_resolution_clock::time_point lastPopTime;


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

	void PopEvent();

	static void PopThread();

};

int recvn(SOCKET s, char *buf, int len, int flags);