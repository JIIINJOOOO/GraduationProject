// Fill out your copyright notice in the Description page of Project Settings.


#include "Network.h"
HANDLE loginEvent;
Network::Network() {
	int retval;
	WSAData wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) return;

	m_sock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (m_sock == INVALID_SOCKET) return;

	// Set nagle algorithm off
	// BOOL option = TRUE;
	// setsockopt(m_sock, IPPROTO_TCP, TCP_NODELAY, (char*)& option, sizeof(option));
	m_status = p_free;

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

	HANDLE rthread = CreateThread(NULL, 0, RecvThread, this, 0, NULL);

}

DWORD WINAPI Network::RecvThread(LPVOID p) {
	int retval;
	Network* arg = reinterpret_cast<Network*>(p);
	SOCKET sock = arg->m_sock;
	int recvBytes = 0;
	while (true) {

		// retval = recv(sock, (char*)&recvBytes, sizeof(int), 0);
		// retval = recv(sock, arg->recvBuf, recvBytes, 0);

		retval = recvn(sock, arg->recvBuf, BUFSIZE, 0);
		// if (retval == SOCKET_ERROR) break;
		// if (retval == 0) break;

		arg->recvBuf[retval] = '\0';

		arg->ProcessPacket(arg->recvBuf);
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
		m_status = p_login;
	}
					  break;
	case sc_login_fail:
		break;
	case sc_signup_ok:
		break;
	case sc_signup_fail:
		break;
	case sc_leave:
		break;
	case login_packet: {
		SC_LOGIN* pack = reinterpret_cast<SC_LOGIN*>(buf);
		if (pack->state) m_status = p_login;
	}
					   break;
	default:
		break;
	}
}

void Network::SetAccountInfo(const char* id, const char* pass) {
	this->id.clear();
	this->pass.clear();
	this->id = id;
	this->pass = pass;
}

int Network::GetStatus() const {
	return m_status;
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