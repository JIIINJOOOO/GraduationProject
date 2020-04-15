#include "CServer.h"
#include "packet.h"
#define NOT_JOIN_ANY_ROOM -1
#define NOT_LOGIN -1
#define SUB_SUCCESS 0
HANDLE g_sendEvents[MAX_PLAYER];
short board[SIDE_LEN][SIDE_LEN];
DWORD WINAPI CServer::WorkerThread(LPVOID arg) {
	CServer* server = (CServer*)arg;
	SOCKET sock = server->GetSock();
	int retval;
	char buf[BUFSIZE];
	int sendBytes = 0;
	int recvBytes = 0;

	// DWORD cbTransferred;
	// ULONG key;
	// PULONG p_key = &key;
	// WSAOVERLAPPED *p_over;

	while (true) {
		// GetQueuedCompletionStatus(server->m_iocp, &cbTransferred, (PULONG_PTR)p_key, &p_over, INFINITE);
		// sock = server->m_clients[key]->sock;
		// if (cbTransferred == 0) {
		// 	closesocket(sock);
		// 	server->m_clients.erase(key);
		// 	continue;
		// }
		// Receive Packet from Client
		retval = recv(sock, (char*)&recvBytes, sizeof(int), 0);
		retval = recv(sock, buf, recvBytes, 0);
		if (retval == SOCKET_ERROR) {
			server->err_display("recv");
			break;
		}
		if (retval == 0) break;
		retval = server->ProcessPacket(buf, NULL);
		if (retval == true) server->Send(buf);
		/*
		buf[recvBytes] = '\0';
		// Casting & Process Packet
		if (buf[0] == login_packet || buf[0] == signup_packet) {
			// Connect DB
			server->m_dbc.AllocateHandle();
			server->m_dbc.ConnectDataSource();

			if (buf[0] == signup_packet)
				retval = server->SignUp(buf);
			if (buf[0] == login_packet) {
				retval = server->Login(buf);
				if (retval != ADD_FAIL) {
					int my_idx = retval;
					server->m_players[my_idx]->SetState(play_game);
					server->m_obj[my_idx] = new CObject;
					server->m_obj[my_idx]->Initialize(Position(), Velocity(), Volume(), Accel(), obj_player);
				}
			}
			sendBytes = sizeof(SC_LOGIN);

			// Disconnect DB
			server->m_dbc.DisconnectDataSource();
		}
		else {
			if (buf[0] == move_packet) {
				CS_MOVE* csUPlayer = (CS_MOVE*)buf;
				server->m_obj[csUPlayer->idx]->SetPosition(csUPlayer->destination);
				// Pos p{ (short)csUPlayer->destination.x, (short)csUPlayer->destination.y };
				// cout << (short)csUPlayer->destination.x << "\t" << (short)csUPlayer->destination.y << "\t" <<
				// 	board[(short)csUPlayer->destination.x + 1807][(short)csUPlayer->destination.y + 1407] << endl;
			}
			if (buf[0] == act_packet) {
				CS_ACT *csAct = (CS_ACT*)buf;
				if (csAct->detailType == attack_packet) {
					server->m_players[csAct->idx]->Attck();
				}
				if (csAct->detailType == skill_packet) {

				}
			}
			SC_UPDATE_OBJ scUObj;
			sendBytes = 0;
			for (int i = 0; i < MAX_OBJ; ++i) {
				if (server->m_obj[i] == NULL) continue;
				Position otherPos = server->m_obj[i]->GetPosition();
				//if (server->m_obj[i]->GetDistance(otherPos) < MAX_VIEW_RANGE) {
					scUObj.type = server->m_obj[i]->GetType();
					scUObj.pos = server->m_obj[i]->GetPosition();
					// scUObj.pos.z = 228.f;
					memcpy(buf + sendBytes, &scUObj, sizeof(SC_UPDATE_OBJ));
					sendBytes += sizeof(SC_UPDATE_OBJ);
				// }
			}
		}
		*/
		
		// // Send Packet to Client
		// retval = send(sock, (char*)&sendBytes, sizeof(int), 0);
		// retval = send(sock, buf, sendBytes, 0);
		// if (retval == SOCKET_ERROR) { 
		// 	server->err_display("send"); 
		// 	break;
		// }
		// if (retval == 0) break;
		// printf("end of cycle of While\n");
	}
	
	closesocket(sock);	
	return 0;
}

DWORD WINAPI CServer::SendThread(LPVOID arg) {
	CServer *Arg = (CServer*)arg;
	int retval;
	while (true) {
		WaitForMultipleObjects(MAX_PLAYER, g_sendEvents, FALSE, INFINITE);
		for (int i = 0; i < MAX_PLAYER; ++i)
			if (Arg->m_clients[i] != NULL) {
				retval = send(Arg->m_clients[i]->sock, (char*)&Arg->sendBytes, sizeof(int), 0);
				retval = send(Arg->m_clients[i]->sock, Arg->buf, Arg->sendBytes, 0);
				if (retval == SOCKET_ERROR) {
					Arg->err_display("send");
					break;
				}
				if (retval == 0) break;
			}
	}
	return 0;
}

DWORD WINAPI CServer::MonsterThread(LPVOID arg) {
	CServer* Arg = (CServer*)arg;
	Arg->CreateMonsters();
	while (true) {
		if (Arg->m_numofUsers < 1) continue;
		for (int i = 0; i < MAX_MONSTER; ++i) {
			short idx = START_POINT_MONSTER + i;
			if (Arg->m_monsters[idx] == NULL) continue;
			if (Arg->m_obj[idx] == NULL) continue;
			if (Arg->m_monsters[idx]->GetHealthPoint() < 0) continue;
			int distance;
			int firstNearPlayerIdx{ 0 };
			// 가장 가까운 플레이어 탐색 (시야 내에서)
			for (int j = 0; j < MAX_PLAYER; ++j) {
				if (Arg->m_obj[j] == NULL) continue;
				if (Arg->m_players[j] == NULL) continue;
				distance = Arg->m_obj[idx]->GetDistance(Arg->m_obj[j]->GetPosition());
				// if (firstNearPlayerIdx == -1 && distance < CHASE_RANGE) firstNearPlayerIdx = j;
				// else continue;
				if (distance > Arg->m_obj[idx]->GetDistance(Arg->m_obj[firstNearPlayerIdx]->GetPosition())) 
					continue;
				if (distance < CHASE_RANGE) firstNearPlayerIdx = j;
			}
			if (firstNearPlayerIdx == -1) {
				if (Arg->m_obj[idx]->GetDistance(Arg->m_obj[idx]->GetPosition())) 
					Arg->m_monsters[idx]->SetState(return_home);
				else  Arg->m_monsters[idx]->SetState(idle);
				continue;
			}
			// 가장 가까운 플레이어와의 거리
			distance = Arg->m_obj[idx]->GetDistance(Arg->m_obj[firstNearPlayerIdx]->GetPosition());
			
			if (distance < ATTACK_RANGE) {
				Arg->m_monsters[idx]->SetState(attack);
			}
			else if (Arg->m_obj[idx]->GetDistance(Arg->m_obj[idx]->GetPosition()))
				Arg->m_monsters[idx]->SetState(return_home);
			else if (Arg->m_monsters[idx]->GetState() != return_home && distance < CHASE_RANGE)
				Arg->m_monsters[idx]->SetState(chase);
			Arg->m_monsters[idx]->Update(*Arg->m_obj[idx], *Arg->m_obj[firstNearPlayerIdx]);
			
			// Test
			// cout << "Start find path" << endl;
			// CObject o;
			// Position p{ 0, 0, 218 };
			// o.Initialize(p, Velocity(), Volume(), Accel(), obj_player);
			// 
			// Arg->m_monsters[idx]->SetState(chase);
			// Arg->m_monsters[idx]->Update(*Arg->m_obj[idx], o);
		}
		Sleep(1000 / 60);
	}
	return 0;
}

bool CServer::ProcessPacket(char* buf, const int& idx) {
	int type = (int)buf[1];
	buf[(int)buf[0]] = 0;
	int retval;
	switch (type) {
	case login_packet:
	case signup_packet: {
		// Connect DB
		m_dbc.AllocateHandle();
		m_dbc.ConnectDataSource();
		if (type == signup_packet)
			retval = SignUp(buf);
		if (type == login_packet) {
			retval = Login(buf);
			if (retval != ADD_FAIL) {
				m_players[idx]->SetState(play_game);
				m_obj[idx] = new CObject;
				m_obj[idx]->Initialize(Position(), Velocity(), Volume(), Accel(), obj_player);
			}
		}
		// sendBytes = sizeof(SC_LOGIN);
		// Disconnect DB
		m_dbc.DisconnectDataSource();
		return true;
	}
	case move_packet: {
		CS_MOVE* csUPlayer = (CS_MOVE*)buf;
		m_obj[csUPlayer->idx]->SetPosition(csUPlayer->destination);
		SC_UPDATE_OBJ scUObj;
		sendBytes = 0;
		for (int i = 0; i < MAX_OBJ; ++i) {
			if (m_obj[i] == NULL) continue;
			scUObj.size = sizeof(SC_UPDATE_OBJ);
			scUObj.type = m_obj[i]->GetType();
			scUObj.pos = m_obj[i]->GetPosition();
			memcpy(buf + sendBytes, &scUObj, sizeof(SC_UPDATE_OBJ));
			sendBytes += sizeof(SC_UPDATE_OBJ);
		}
		buf[0] = sendBytes;
		return true;
	}
	default:
		break;
	}
		return false;
}

void CServer::Send(const char* buf) {
	int retval;
	int sendBytes = (int)buf[0];
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (m_clients[i] == NULL) continue;
		retval = send(m_clients[i]->sock, (char*)&sendBytes, sizeof(int), 0);
		retval = send(m_clients[i]->sock, buf, sendBytes, 0);
		if (retval == SOCKET_ERROR) {
			err_display("send()");
			Disconnect(i);
		}
		if (retval == 0) Disconnect(i);
	}
}

void CServer::Disconnect(const int& idx) {
	if (m_clients[idx] == NULL) return;
	if (0 < idx || MAX_PLAYER < idx) return;
	printf("%s is Log-out in Server (index is %d)\n", m_players[idx]->GetID(), idx);             
	delete m_clients[idx];
	m_clients[idx] = NULL;
	delete m_players[idx];
	m_players[idx] = NULL;
	delete m_obj[idx];
	m_obj[idx] = NULL;
}

void CServer::Start() {
	int retval;
	if (WSAStartup(MAKEWORD(2, 2), &m_wsa) != 0) return;

	m_listensocket = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_listensocket == SOCKET_ERROR) err_quit("WSASocket");

	// Set nagle algorithm off
	BOOL option = TRUE;
	setsockopt(m_listensocket, IPPROTO_TCP, TCP_NODELAY, (char*)& option, sizeof(option));

	ZeroMemory(&m_serveraddr, sizeof(m_serveraddr));
	m_serveraddr.sin_family = AF_INET;
	m_serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	m_serveraddr.sin_port = htons(SERVERPORT);
	retval = ::bind(m_listensocket, (SOCKADDR*)&m_serveraddr, sizeof(m_serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind");

	retval = listen(m_listensocket, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen");

	// m_iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, 0);
	// if (m_iocp = NULL) return;
	// DWORD flags;
	// SYSTEM_INFO si;
	// GetSystemInfo(&si);
	// for (int i = 0; i < (int)si.dwNumberOfProcessors * 2; ++i) {
	// 	m_hthread = CreateThread(NULL, 0, WorkerThread, (LPVOID)this, 0, NULL);
	// 	if (m_hthread == NULL) return;
	// 	CloseHandle(m_hthread);
	// }
	for (short x = -1807; x < 1007; ++x)
		for (short y = -1407; y < 1407; ++y) {
			float z;
			if (-410 < x &&x < -137 && 287 < y&& y < 495) {
				// 우측에서 처음 올라가는 계단
				z = (188.0 / 273.0)*x + (423.0 + (137.0 * 188.0) / 273.0);
			}
			else if (-100 < x && x<100 && -530<y && y<-300) {
				// 좌측 가장 높은 곳으로 올라가는 오르막길
				z = (-8.0 / 23.0)*y + 423.0 - (2400.0 / 23.0);
			}
			else if (-100 < x && x < 100 && -730 < y < -530) {
				// 좌측 가장 높은 정사각형 지대
				z = 503.f;
			}
			else if (170 < x && x < 730 && 30 < y && y < 130) {
				// 가운데 볼록 튀어나온거
				z = 452.f;
			}
			else if (-110 < x &&x < 730 && -300 < y && y < -110) {
				// 왼쪽
				z = 418.f;
			}
			else if (-100 < x && x < 730 && 287 < y&& y < 495) {
				// 오른쪽 ?
				z = 418.f;
			}
			else if (-100 < x && x < 730 && 130 < y&&y < 214) {
				// 오른쪽 작은 경사
				z = (-17.0 / 42.0)*y + 452.0 + (17.0 / 42.0) * 130;
			}
			else if (-100 < x && x < 730 && -110 < y&& y < 30) {
				// 왼쪽 작은 경사
				z = (17.0 / 70.0)*y + 452.0 - (51.0 / 7.0);
			}
			else if (-170 < x && x < 700 && -300 < y&&y < 460)
				z = 418.f;
			else z = 228.f;
			board[x + X_SIDE][y + Y_SIDE] = z;
		}
	printf("Create Map Complete\n");
	m_hthread = CreateThread(NULL, 0, MonsterThread, (LPVOID)this, 0, NULL);

	while (true) {
		int addrlen = sizeof(m_clientaddr);
		m_clientsock = accept(m_listensocket, (SOCKADDR*)&m_clientaddr, &addrlen);
		if (m_clientsock == INVALID_SOCKET) {
			err_display("accept");
			break;
		}
		int newIdx = GetNewSockIdx();
		if (newIdx == ADD_FAIL) break;
		m_clients[newIdx] = new SOCKETINFO;
		ZeroMemory(&m_clients[newIdx]->overlapped, sizeof(m_clients[newIdx]->overlapped));
		m_clients[newIdx]->sock = m_clientsock;
		m_clients[newIdx]->idx = newIdx;
		// m_clients[newIdx]->wsabuf[0].buf = m_clients[newIdx]->buf;
		// m_clients[newIdx]->wsabuf[0].len = BUFSIZE;
		// flags = 0;
		// CreateIoCompletionPort((HANDLE)m_clientsock, m_iocp, m_clientsock, 0);

		// send access sucess (player's index)
		retval = send(m_clients[newIdx]->sock, (char*)&newIdx, sizeof(newIdx), 0);
		if (retval == SOCKET_ERROR) {
			err_display("send access");
			break;
		}
		if (retval == 0) break;
		printf("Client is Access Server - Index is %d\n", newIdx);

		// retval = WSARecv(m_clientsock, m_clients[newIdx]->wsabuf, 1, NULL,
		// 	&flags, &(m_clients[newIdx]->overlapped), NULL);
		// if (retval == SOCKET_ERROR) {
		// 	if (WSAGetLastError() != ERROR_IO_PENDING)
		// 		err_display("WSARecv");
		// }
		m_hthread = CreateThread(NULL, 0, WorkerThread, (LPVOID)this, 0, NULL);
		if (m_hthread == NULL) closesocket(m_clientsock);
		else CloseHandle(m_hthread);
	}
	closesocket(m_listensocket);
	WSACleanup();
}

int CServer::Login(char* buf) {
	int retval;
	buf[sizeof(CS_LOGIN)] = '\0';
	CS_LOGIN *csLogin = (CS_LOGIN*)buf;
	int playerIdx = csLogin->userIdx;
	
	std::string sql = "select * from Account where id  = \'" + (std::string)csLogin->id +
		"\' and password = \'" + (std::string)csLogin->password + "\'";
	SC_LOGIN scLogin{ sizeof(SC_LOGIN), login_packet, NULL };
	m_mu.lock();
	retval = m_dbc.ExcuteStatementDirect((SQLCHAR*)sql.c_str());
	if (m_dbc.RetrieveResult(csLogin->id, csLogin->password) == false)
		scLogin.state = login_error;
	m_mu.unlock();
	printf("%s\n", csLogin->id);
	if (retval == SQL_EXUTE_OK && scLogin.state != login_error) {
		scLogin.state = login_ok;
		m_players[playerIdx] = new CPlayer;
		m_players[playerIdx]->Initialize(CPlayer(std::string(csLogin->id), std::string(csLogin->password)));
		// g_players[playerIdx]->Initialize(CPlayer(std::string(csLogin->id), std::string(csLogin->password)));
		// playerIdx = 0;
		// printf("%s, %s\n", csLogin->id, csLogin->password);
	
		// m_players[0]->Initialize(CPlayer(std::string(csLogin->id), std::string(csLogin->password)));

		printf("%s is Log-in Success\n", csLogin->id);
	}
	else {
		scLogin.state = login_error;
		printf("%s is Log-in Fail (Invalid ID or Password)\n", csLogin->id);

	}
	m_numofUsers++;
	ZeroMemory(buf, BUFSIZE);
	memcpy(buf, &scLogin, sizeof(SC_LOGIN));
	return playerIdx;
}

int CServer::SignUp(char* buf) {
	int retval;
	buf[sizeof(CS_LOGIN)] = '\0';
	CS_LOGIN *csSign = (CS_LOGIN*)buf;
	std::string sql = "INSERT INTO Account VALUES (\'" + (std::string)csSign->id +
		"\', \'" + (std::string)csSign->password+ "\')";
	printf("%s \n", sql);

	SC_LOGIN scSign{ signup_packet, NULL };
	retval = m_dbc.ExcuteStatementDirect((SQLCHAR*)sql.c_str());
	if (retval == SQL_EXUTE_OK) {
		scSign.state = signup_ok;
		printf("Create New Account - ID : %s \n", csSign->id);
	}
	else {
		scSign.state = signup_error;
		printf("Create New Account Fail - ID : %s \n", csSign->id);
	}
	ZeroMemory(buf, BUFSIZE);
	memcpy(buf, &scSign, sizeof(SC_LOGIN));
	return scSign.state;
}

int CServer::AddPlayer() {
	int idx{ -1 };
	for (int i = 0; i < MAX_CLIENTS; ++i)
		if (m_players[i] == NULL) {
			idx = i;
			break;
		}
	if (idx == ADD_FAIL) {
		printf("Can't Add Player \n");
		return ADD_FAIL;
	}
	return idx;

}

int CServer::SubPlayer(const int& idx) {
	if (idx < 0 || idx >= MAX_CLIENTS) return ADD_FAIL;

	printf("%s is leave Game - (idx is %d)\n", m_players[idx]->GetID().c_str(), idx);
	m_players[idx] = NULL;
	--m_numofUsers;
	return SUB_SUCCESS;
}

int CServer::GetNewSockIdx() {
	for (int i = 0; i < MAX_PLAYER; ++i)
		if (m_clients[i] == NULL) return i;
	return ADD_FAIL;
}

void CServer::CreateMonsters() {
	Position defPos{-530.f, -10.f, 218.f};
	for (int i = 0; i < MAX_MONSTER; ++i) {
		short idx = START_POINT_MONSTER + i;
		m_monsters[idx] = new CMonster;
		m_monsters[idx]->Initialize(defPos, normal);
		m_obj[idx] = new CObject;
		m_obj[idx]->Initialize(defPos, Velocity(), Volume(), Accel(), obj_monster);
	}
	printf("Create Monsters Complete (num of : %d)\n", MAX_MONSTER);
	// m_monsters[BOSS_IDX] = new CMonster;
	// m_monsters[BOSS_IDX]->Initialize(defPos, boss);
	// m_obj[BOSS_IDX] = new CObject;
	// m_obj[BOSS_IDX]->Initialize(defPos, Velocity(), Volume(), Accel(), obj_monster);
}

CServer::~CServer() {
	m_clients.clear();
	m_players.clear();
	m_monsters.clear();
	m_obj.clear();
	
}


void CServer::err_quit(const char* msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL
	);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

void CServer::err_display(const char* msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL
	);
	printf("[%s] %s", msg, (char*)lpMsgBuf);
	LocalFree(lpMsgBuf);
}