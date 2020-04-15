#pragma once

#pragma comment (lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WS2tcpip.h>
#include <iostream>
#include <vector>
#include <mutex>
#include <map>
#include <thread>
#include "globals.h"
#include "CDBConnector.h"
#include "CPlayer.h"
#include "CMonster.h"

using namespace std;

struct SOCKETINFO {
	OVERLAPPED overlapped;
	SOCKET sock;
	int idx;
	char buf[BUFSIZE];
	int recvBytes;
	int sendBytes;
	WSABUF wsabuf[1];
};

class CServer {
private:
	WSADATA m_wsa;
	HANDLE m_hthread;
	HANDLE m_iocp;
	
	SOCKET m_listensocket;
	SOCKADDR_IN m_serveraddr;

	SOCKET m_clientsock;
	SOCKADDR_IN m_clientaddr;
	map<int, SOCKETINFO*> m_clients;
	map<int, CObject*> m_obj;
	map<int, CPlayer*> m_players;
	map<int, CMonster*> m_monsters;
	
	char buf[BUFSIZE];
	int sendBytes;
	std::mutex m_mu;

	CDBConnector m_dbc;

	int m_numofRooms;
	int m_numofUsers;

public:
	CServer() = default;
	~CServer();

	void Start();
	void CreateMonsters();

	// Show Error Massage+
	void err_quit(const char* msg);
	void err_display(const char* msg);

	// Thread Function
	static DWORD WINAPI WorkerThread(LPVOID arg);
	static DWORD WINAPI SendThread(LPVOID arg);
	static DWORD WINAPI MonsterThread(LPVOID arg);

	bool ProcessPacket(char* buf, const int& idx);
	void Send(const char* buf);
	void Disconnect(const int& idx);
	// Supervise Account with MS-SQL
	int Login(char* buf);
	int SignUp(char* buf);

	// Supervise Player
	int GetNewSockIdx();
	int AddPlayer();
	int SubPlayer(const int& idx);

	// for Test
	SOCKET GetSock() { return m_clientsock; }

};

