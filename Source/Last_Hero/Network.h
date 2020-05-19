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
#include "protocol.h"

#define MAXLEN 10
#define SERVERIP "192.168.10.30"
#define SERVERPORT 9000
#define BUFSIZE 512
#define MAX_USERS 4
/**
 * 
 */
using namespace std;

class LAST_HERO_API Network
{
private:
	SOCKET m_sock;
	HANDLE m_sendEvent;
	char sendBuf[BUFSIZE];
	char recvBuf[BUFSIZE];
	P_STATE m_status = p_free;
	string id, pass;
public:
	HANDLE loginEvent;
public:
	Network();
	~Network() = default;

	void SetAccountInfo(const char* id, const char* pass);
	void ProcessPacket(char* buf);

	void SendPacket(void* packet);

	// static void RecvThread(void* net);
	static DWORD WINAPI RecvThread(LPVOID p);
	
	P_STATE GetStatus() const;
};

int recvn(SOCKET s, char *buf, int len, int flags);