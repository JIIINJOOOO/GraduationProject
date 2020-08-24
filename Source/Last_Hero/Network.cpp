// Fill out your copyright notice in the Description page of Project Settings.


#include "Network.h"
#include "MyGameModeBase.h"
HANDLE gmbEvent;
// GMB_Event gmb;
mutex gmbLock;

Network::Network() {
	// hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	int retval;
	WSAData wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return;

	m_sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_sock == INVALID_SOCKET) return;

	// Set nagle algorithm off
	BOOL option = TRUE;
	setsockopt(m_sock, IPPROTO_TCP, TCP_NODELAY, (char*)& option, sizeof(option));

	m_status = p_free;
	isHost = false;

	//connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(m_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) return;

	m_sendEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	loginEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	gmbEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	HANDLE rthread = CreateThread(NULL, 0, RecvThread, this, 0, NULL);

}

DWORD WINAPI Network::RecvThread(LPVOID p) {
	int retval = 0;
	Network* arg = reinterpret_cast<Network*>(p);
	SOCKET sock = arg->m_sock;
	int recvBytes = 0;
	while (true) {
		// retval = recv(sock, (char*)&recvBytes, sizeof(int), 0);
		// retval = recv(sock, arg->recvBuf, recvBytes, 0);

		retval = recv(sock, arg->recvBuf, BUFSIZE, 0);
		// if (retval == SOCKET_ERROR) break;
		// if (retval == 0) break;

		arg->recvBuf[retval] = '\0';
		arg->ProcessPacket(arg->recvBuf);
		Sleep(1000 / 60);
		// SetEvent(gmbEvent);
	}
	return 0;
}

void Network::SendPacket(void* packet) {
	char* buf = reinterpret_cast<char*>(packet);
	int sendBytes = buf[0];
	int retval = send(m_sock, buf, buf[0], 0);
	// if (retval == SOCKET_ERROR) {}
	// if (retval == 0) {}
}

void Network::ProcessPacket(char* buf) {
	switch (buf[1]) {
	case sc_login_ok: {
		SC_LOGIN_OK* pack = reinterpret_cast<SC_LOGIN_OK*>(buf);
		my_id = pack->uid;
		m_status = p_login;
		SetEvent(loginEvent);

		GMB_Event ev;
		ev.type = sc_login_ok;
		eventLock.lock();
		eventQue.push(ev);
		eventLock.unlock();

	} break;
	case sc_login_fail:
		break;
	case sc_signup_ok:
		break;
	case sc_signup_fail:
		break;
	case sc_leave: {
		SC_LEAVE* pack = reinterpret_cast<SC_LEAVE*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->id;
	}break;
	case sc_enter_obj: {
		SC_OBJECT_ENTER* pack = reinterpret_cast<SC_OBJECT_ENTER*>(buf);

		// 	if (pack->oid == 0) break;
		//	if (pack->oid < 10) {
		GMB_Event ev;
		ev.type = sc_enter_obj;
		ev.pos = pack->pos;
		ev.oid = pack->oid;
		// gmbLock.lock();
		eventLock.lock();
		eventQue.push(ev);
		eventLock.unlock();

		// gmb.type = sc_enter_obj;
		// gmb.pos = pack->pos;
		// gmb.oid = pack->oid;
		// gmbLock.unlock();
			// myGame.PostLogin(&players[pack->oid]);
			// players[pack->oid];
	//	}
	}break;
	case sc_update_obj: {
		SC_UPDATE_OBJ* pack = reinterpret_cast<SC_UPDATE_OBJ*>(buf);
		GMB_Event ev;
		ev.type = sc_update_obj;
		ev.pos = pack->pos;
		ev.oid = pack->oid;
		ev.rotation = pack->rotation;

		eventLock.lock();
		eventQue.push(ev);
		eventLock.unlock();

	}break;
	case login_packet: {
		SC_LOGIN* pack = reinterpret_cast<SC_LOGIN*>(buf);
		if (pack->state) m_status = p_login;
	}break;
	case sc_attack: {
		SC_OBJ_ATTACK* pack = reinterpret_cast<SC_OBJ_ATTACK*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;

		eventLock.lock();
		eventQue.push(ev);
		eventLock.unlock();
	}break;
	case sc_fireball: {
		SC_FIREBALL* pack = reinterpret_cast<SC_FIREBALL*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;

		eventLock.lock();
		eventQue.push(ev);
		eventLock.unlock();
	}break;
	case sc_guard: {
		SC_OBJ_GUARD* pack = reinterpret_cast<SC_OBJ_GUARD*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;

		eventLock.lock();
		eventQue.push(ev);
		eventLock.unlock();
	}break;
	case sc_jump: {
		SC_JUMP* pack = reinterpret_cast<SC_JUMP*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;

		eventLock.lock();
		eventQue.push(ev);
		eventLock.unlock();
	}break;
	case sc_evade: {
		SC_EVADE* pack = reinterpret_cast<SC_EVADE*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;

		eventLock.lock();
		eventQue.push(ev);
		eventLock.unlock();
	}break;
	case sc_weapon_on: {
		SC_WEAPON_ON* pack = reinterpret_cast<SC_WEAPON_ON*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;

		eventLock.lock();
		eventQue.push(ev);
		eventLock.unlock();
	}break;
	case sc_weapon_off: {
		SC_WEAPON_OFF* pack = reinterpret_cast<SC_WEAPON_OFF*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;

		eventLock.lock();
		eventQue.push(ev);
		eventLock.unlock();
	}break;
	default:
		break;
	}
	// SetEvent(gmbEvent);
}

void Network::SetAccountInfo(const char* id, const char* pass) {
	this->id.clear();
	this->pass.clear();
	this->id = id;
	this->pass = pass;
}

int Network::GetStatus() const {
	// return (int)gmb.type;
	return m_status;
}

int Network::GetMyID() const {
	return my_id;
}

void Network::PopEvent() {
	eventLock.lock();
	eventQue.pop();
	eventLock.unlock();
}

int recvn(SOCKET s, char *buf, int len, int flags) {
	int received;
	char *ptr = buf;
	int  left = len;

	while (left > 0) {
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;

		left -= received;
		ptr += received;
	}
	return (len - left);
}