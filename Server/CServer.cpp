#include "CServer.h"
#include "packet.h"

#define NOT_JOIN_ANY_ROOM -1
#define NOT_LOGIN -1

HANDLE g_sendEvents[MAX_PLAYER];

DWORD WINAPI CServer::WorkerThread(LPVOID arg) {
	CServer* server = (CServer*)arg;
	SOCKET sock = server->GetSock();
	SOCKADDR_IN clientaddr;

	int retval;
	char buf[BUFSIZE];
	int sendBytes = 0;
	int recvBytes = 0;

	// int addrlen = sizeof(clientaddr);
	// getpeername(sock, (SOCKADDR*)&clientaddr, &addrlen);
	DWORD cbTransferred;
	ULONG key;
	PULONG p_key = &key;
	WSAOVERLAPPED *p_over;

	while (true) {
		// GetQueuedCompletionStatus(server->m_iocp, &cbTransferred, (PULONG_PTR)p_key, &p_over, INFINITE);
		// sock = server->m_clients[key]->sock;
		// if (cbTransferred == 0) {
		// 	closesocket(sock);
		// 	server->m_clients.erase(key);
		// 	// �÷��̾� �������ϱ� ���⼭ ���� �� ��������ϳ�?
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
		buf[recvBytes] = '\0';
		// Casting & Process Packet
		/*
		else if (g_players[my_idx]->GetState() == In_Lobby) {
			if (buf[0] != lobby_packet) break;
			buf[recvBytes] = '\0';
			CS_LOBBY *csLobby = (CS_LOBBY*)buf;

			// 2 - 1 �� ���� 
			if (csLobby->state == create_room) {
				int rnum = server->AddRoom(my_idx);
				// rnum�� �����ڵ� ��ȯ������ �������� �߰�����
				g_scLobby[rnum].state = create_room;
				g_scLobby[rnum].type = lobby_packet;
				my_room = rnum;
				Sleep(1000/60);	// Stay 1 frame. for Update user count

				// Create Struct To Buf
				sendBytes = sizeof(SC_LOBBY);
				ZeroMemory(buf, BUFSIZE);
				memcpy(buf, &g_scLobby[rnum], sendBytes);
			}
			// 2 - 2 �� ����
			if (csLobby->state == join_room) {
				SC_LOBBY scLobby;
				if (0 < server->m_lobby[csLobby->num]->GetUsers() && server->m_lobby[csLobby->num]->GetUsers() < MAX_PLAYER) {
					// my_idx = server->m_lobby[csLobby->num].AddUser(player);
					// �̰� ���� ���� ������ �־ �׷��� ó���ع��ȴµ� ���� �׳� �ε����� �̿��ؼ� ���������� �׼����ϰԲ� �ع��ȴ�
					server->m_lobby[csLobby->num]->AddUser(my_idx);
					g_players[my_idx]->SetState(none_ready);
					// player.SetState(none_ready);
					my_room = csLobby->num;
				
					scLobby = server->m_lobby[my_room]->MakeLobbyPack(join_room);
					scLobby.idx = my_room;
					
					// ���� �� �ȿ��� idx���� �κ��� idx�� �־���µ� ��𼱰� ���� �߸��Ǿ��ִµ��ϴ�
					// ��� ���� �־������?
					// �ϴ� �ӽ� ��������
					// �ؿ��� �ѹ� �� �־�����
					
					// scLobby = g_scLobby[my_room];

				}
				else {
					scLobby.type = lobby_packet;
					scLobby.idx = NOT_JOIN_ANY_ROOM;
					scLobby.users = 0;
					scLobby.state = join_room;
				}
				Sleep(1000 / 60);	// Stay 1 frame. for Update user count

				// Create Struct To Buf
				sendBytes = sizeof(SC_LOBBY);
				ZeroMemory(buf, BUFSIZE);
				memcpy(buf, &scLobby, sendBytes);
			}
			// 2 - 3 �� ������Ʈ�ޱ�
			if (csLobby->state == update_room) {
				//
				// ����� �����丵�� �ڵ忡 �°� ���� �ʿ���
				//
				// printf("������� �Գ�\n");
				int cnt{0};
				ZeroMemory(g_scLobby, sizeof(g_scLobby));
				for (int i = 0; i < MAX_LOBBY; ++i) 
					if (server->m_lobby[i] != NULL) {
						g_scLobby[cnt] = server->m_lobby[i]->MakeLobbyPack(update_room);
						++cnt;
					}
				// g_scLobby[0].state = update_room;
				sendBytes = cnt * sizeof(SC_LOBBY);
				ZeroMemory(buf, BUFSIZE);
				memcpy(buf, &g_scLobby, sendBytes);
			}
		}
		else if (g_players[my_idx]->GetState() == host || g_players[my_idx]->GetState() == none_ready || g_players[my_idx]->GetState() == ready) {
			buf[recvBytes] = '\0';
			CS_LOBBY *csLobby = (CS_LOBBY*)buf;
			SC_LOBBY scLobby;
			
			// ���� ������ ���� �����ؼ� ���°Ÿ� ������µ�
			// ������ �濡 ������ �ϸ� �� ���� ��ɾ� ġ�� �ְ� �ñ��			
			
			// 1 �� �ȿ��� ����ϱ�
			// 1 - 2 �濡�� ������
			if (csLobby->state == out_room) {
				retval = server->m_lobby[csLobby->num]->SubUsers(my_idx);
				if (retval == DELETE_ROOM) {
					delete server->m_lobby[csLobby->num];
					server->m_lobby[csLobby->num] = NULL;
				}
				g_players[my_idx]->SetState(In_Lobby);
				scLobby.type = lobby_packet;
				scLobby.users = -1;
				// Create struct to buf
				sendBytes = sizeof(SC_LOBBY);
				ZeroMemory(buf, BUFSIZE);
				memcpy(buf, &scLobby, sendBytes);
				// ��Ŷ state ������ ���� �߸� �־ Ŭ�󿡼� join���� ���. �۵��� �������� ����
			}

			// 1 - 3 ���� �÷��̾� ���� ������
			// �÷��̾� ��Ŷ ����ü�� �κ� ���� �÷��̾��� ����ŭ �����Ͽ� ������.
			else if (csLobby->state == update_player) {
				SC_PLAYER scPlayer[MAX_PLAYER];
				printf("%d szjkf\n", csLobby->num);
				for (int i = 0; i < MAX_PLAYER; ++i) {
					int idx = server->m_lobby[csLobby->num]->GetPlayerIdx(i);
					scPlayer[i].type = player_packet;
					scPlayer[i].idx = idx;
					if (idx == None) continue;
					scPlayer[i].state = g_players[idx]->GetState();
					strcpy_s(scPlayer[i].id, g_players[idx]->GetID().c_str());
					printf("%d : %s \n", i, scPlayer[i].id);
					// scPlayer[i] = server->m_lobby[csLobby->num]->GetPlayer2Pack(i);
					// ���� ���⼭ �κ� ��ȣ�� ����� ���޾ƿ�������
					// ������ ������
					
					// ���� ���ϱ� 1�� 2�� ������ ������Ʈ ȣ���ϸ�
					// ����� ���۵� �ٵ� 2�� ������
					// ������ 2�� �����ִ°ɷ� ǥ���
					// Ŭ�󿡼� �ʱ�ȭ ����� �ȵǰ� �ִµ� �̰� ���� ��������
					
				}
				// Create struct to buf
				sendBytes = sizeof(scPlayer);
				ZeroMemory(buf, BUFSIZE);
				memcpy(buf, &scPlayer, sendBytes);
			}
			
			// ������ ���¿��� ���� ������Ʈ�� ��û�ϸ� ��Ŷ �� �տ�
			// player��Ŷ�� ��Ÿ���� ���� �ƴ϶� lobby ��Ŷ�� ��Ÿ���� ���� ����
			// �̰� �� �� �̷��µ� ���� ������ ��¥ �ž�

			
			// 2 ���� ����
			
			// 2 - 1 ����ڱ�
			else if (csLobby->state == ready) {
				server->m_lobby[csLobby->num]->ReadyPlayer(my_idx);	
				SC_PLAYER scPlayer;
				scPlayer.type = player_packet;
				scPlayer.state = ready;
				// player.SetState(ready);
				g_players[my_idx]->SetState(ready);
				sendBytes = sizeof(SC_PLAYER);
				ZeroMemory(buf, BUFSIZE);
				memcpy(buf, &scPlayer, sendBytes);
				printf("%s is Ready\n", g_players[my_idx]->GetID().c_str());
			}
			// 2 - 2 ���ӽ��� (����)
			else if (csLobby->state = start) {
				// 1 �÷��̾ ȣ��Ʈ�ΰ�
				if (server->m_lobby[csLobby->num]->IsHost(my_idx)) {
					// 2 ��� �÷��̾ ready �����ΰ�
						// ������� ������ start�� Ȯ���ǹǷ� �������� ������ �κ�迭 �ǵ���� �����̴�.
						// �� ������ ������� �ǵ帮�� �ڵ������� �ٲ��ְڱ���
						// �̰� �� �ƴҶ� ó�����ִ� �͵� �����ѵ�
					SC_START scStart;
					scStart.type = start_packet;
					if (server->m_lobby[csLobby->num]->Start()) {
						scLobby.state = start_ok;
						for (int i = 0; i < MAX_PLAYER; ++i) {
							// ������Ʈ �߰�
							int playerIdx = server->m_lobby[csLobby->num]->GetPlayerIdx(i);
							if (playerIdx == None) scStart.users[i] = None;
							else scStart.users[i] = i;
							
							// g_players[playerIdx]->AddObject(Position(), Velocity(), Volume(), Accel(), obj_player);
						// 	int playerIdx = server->m_lobby[csLobby->num]->GetPlayerIdx(i);
						// 	if (playerIdx == None) continue;
						// 	int objIdx = server->m_lobby[csLobby->num]->AddObject(Position(), Velocity// (), Volume(), Accel(), obj_player);
						// 	server->m_lobby[csLobby->num]->SetObject(i, objIdx);
						
							
						}

					}
					else scStart.state = start_fail;
					// scLobby.state = start_ok;
				}
				


				ZeroMemory(buf, BUFSIZE);
				sendBytes = sizeof(SC_LOBBY);
				memcpy(buf, &scLobby, sendBytes);
						
						�̰� �� ���ӽ����̴ϱ�					
						�÷��̾���� ���¸� ��� ���������� �ٲٰ�
						���� ���µ� ���������� �ٲ���
						�� �׷��� �κ� ����϶� ������� ������ �������� ������ �װ͵� �Ǻ� �����ؾ߰ڴ�
						
				
			}
		}
		else if (g_players[my_idx]->GetState() == In_Game) {
	
		buf[recvBytes] = '\0';
		CS_PLAYER *csPlayer = (CS_PLAYER*)buf;
		

		
		�̵�
		���� �˻�
		����, ȼ�� �� �ൿ�� ���� ���
		�浹üũ
		

		// server->m_lobby[my_room]->Update();	// �ϴ� ���� �δµ� ���� ������� ������ �𸧤�
		SC_PLAYER scPlayer[MAX_PLAYER];
		for (int i = 0; i < MAX_PLAYER; ++i) {
			int idx = server->m_lobby[my_room]->GetPlayerIdx(i);
			if (idx == None) continue;
			scPlayer[i].type = player_packet;
			scPlayer[i].idx = i;
			scPlayer[i].state = g_players[idx]->GetState();
			strcpy_s(scPlayer[i].id, g_players[idx]->GetID().c_str());
		}
		ZeroMemory(buf, BUFSIZE);
		sendBytes = sizeof(scPlayer);
		memcpy(buf, &scPlayer, sendBytes);
		}
		*/
		
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
				//Position retPos = server->m_obj[csUPlayer->idx]->GetPosition();
				//Position detPos = csUPlayer->destination;
			//	printf("%f %f %f -> %f %f %f", retPos.x, retPos.y, retPos.z, detPos.x, detPos.y, detPos.z);
				server->m_obj[csUPlayer->idx]->SetPosition(csUPlayer->destination);
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
				if (server->m_obj[i]->GetDistance(otherPos) < MAX_VIEW_RANGE) {
					scUObj.type = server->m_obj[i]->GetType();
					scUObj.pos = server->m_obj[i]->GetPosition();
					memcpy(buf + sendBytes, &scUObj, sizeof(SC_UPDATE_OBJ));
					sendBytes += sizeof(SC_UPDATE_OBJ);
				}
			}
		}
		// Send Packet to Client
		retval = send(sock, (char*)&sendBytes, sizeof(int), 0);
		retval = send(sock, buf, sendBytes, 0);
		if (retval == SOCKET_ERROR) { 
			server->err_display("send"); 
			break;
		}
		if (retval == 0) break;

		// printf("end of cycle of While\n");
	}
	// printf("%s is Log-Out\n", server->m_players[my_idx]->GetID().c_str());
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
		for (int i = 0; i < MAX_MONSTER; ++i) {
			short idx = START_POINT_MONSTER + i;
			if (Arg->m_monsters[idx]->GetHealthPoint() < 0) continue;
			// Arg->m_obj[idx]->GetPosition()
		}
	}
	return 0;
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
	printf("%s \n", sql);
	SC_LOGIN scLogin{ login_packet, NULL };
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

int CServer::AddRoom(const int& hostIdx) {
	int idx{ -1 };
	for (int i =0; i<MAX_LOBBY; ++i)
		if (m_lobby[i] == NULL) {
			idx = i;
			break;
		}

	if (idx == ADD_FAIL) {
		printf("Can't Add Room \n");
		return ADD_FAIL;
	}

//	printf("Create Room - %d \t host : %s \n", idx, g_players[hostIdx]->GetID().c_str());
	m_lobby[idx] = new CLobby;
	m_lobby[idx]->SetIdx(idx);
	m_lobby[idx]->AddUser(hostIdx);
	m_lobby[idx]->SetHost(hostIdx);
	// g_players[hostIdx]->SetState(host);
	return idx;
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
	Position defPos;
	for (int i = 0; i < MAX_MONITORS; ++i) {
		short idx = START_POINT_MONSTER + i;
		m_monsters[idx] = new CMonster;
		m_monsters[idx]->Initialize(defPos, normal);
		m_obj[idx] = new CObject;
		m_obj[idx]->Initialize(defPos, Velocity(), Volume(), Accel(), obj_monster);
	}
	m_monsters[BOSS_IDX] = new CMonster;
	m_monsters[BOSS_IDX]->Initialize(defPos, boss);
	m_obj[BOSS_IDX] = new CObject;
	m_obj[BOSS_IDX]->Initialize(defPos, Velocity(), Volume(), Accel(), obj_monster);

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