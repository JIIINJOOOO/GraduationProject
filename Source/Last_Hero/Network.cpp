// Fill out your copyright notice in the Description page of Project Settings.


#include "Network.h"
#include "MyGameModeBase.h"
extern Network net;

Network::Network() {
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
	isMovingN = false;
	wpnType = wpn_none;

	//objEventQue.insert(GMB_ID);

	//connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(m_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) return;

	// loginEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
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
		retval = recvn(sock, arg->recvBuf, BUFSIZE, 0);
		// if (retval == SOCKET_ERROR) break;
		// if (retval == 0) break;

		arg->recvBuf[retval] = '\0';

		arg->ProcessPacket(arg->recvBuf);
		// Sleep(1000 / 60);
		// SetEvent(gmbEvent);
	}
	return 0;
}

void Network::PopThread() {
	while (true) {
		if (net.eventQue.empty()) this_thread::sleep_for(1ms);
		if (net.lastPopTime + 1s > high_resolution_clock::now()) {
			auto& ev = net.eventQue.front();
			CS_SYNC_FAIL pack{ sizeof(CS_SYNC_FAIL), cs_sync_fail, ev.oid };
			net.SendPacket(&pack);
			net.PopEvent();
		}
	}
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
		// set loginevent

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

		objEventQue[pack->id].push(ev);
	}break;
	case sc_enter_obj: {
		SC_OBJECT_ENTER* pack = reinterpret_cast<SC_OBJECT_ENTER*>(buf);

		GMB_Event ev;
		ev.type = sc_enter_obj;
		ev.pos = pack->pos;
		ev.oid = pack->oid;
		ev.o_type = pack->o_type;

		objEventQue[GMB_ID].push(ev);
	}break;
	case sc_update_obj: {
		SC_UPDATE_OBJ* pack = reinterpret_cast<SC_UPDATE_OBJ*>(buf);
		GMB_Event ev;
		ev.type = sc_update_obj;
		ev.pos = pack->pos;
		ev.oid = pack->oid;
		ev.rotation = pack->rotation;
		ev.velocity = pack->velocity;

		objEventQue[pack->oid].push(ev);
	}break;
	case sc_attack: {
		SC_OBJ_ATTACK* pack = reinterpret_cast<SC_OBJ_ATTACK*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;
		ev.mp = static_cast<short>(pack->combo);	// 

		objEventQue[pack->oid].push(ev);
	}break;
	case sc_fireball: {
		SC_FIREBALL* pack = reinterpret_cast<SC_FIREBALL*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;

		objEventQue[pack->oid].push(ev);
	}break;
	case sc_guard: {
		SC_OBJ_GUARD* pack = reinterpret_cast<SC_OBJ_GUARD*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;

		objEventQue[pack->oid].push(ev);
	}break;
	case sc_jump: {
		SC_JUMP* pack = reinterpret_cast<SC_JUMP*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;

		objEventQue[pack->oid].push(ev);
	}break;
	case sc_evade: {
		SC_EVADE* pack = reinterpret_cast<SC_EVADE*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;

		objEventQue[pack->oid].push(ev);
	}break;
	case sc_weapon_on: {
		SC_WEAPON_ON* pack = reinterpret_cast<SC_WEAPON_ON*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;

		objEventQue[pack->oid].push(ev);
	}break;
	case sc_weapon_off: {
		SC_WEAPON_OFF* pack = reinterpret_cast<SC_WEAPON_OFF*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;

		objEventQue[pack->oid].push(ev);
	}break;
	case sc_level_up: {
		SC_LEVEL_UP* pack = reinterpret_cast<SC_LEVEL_UP*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;
		ev.hp = pack->hp;
		ev.mp = pack->mp;
		ev.level = pack->level;
		ev.exp = pack->exp;

		objEventQue[pack->oid].push(ev);
	}break;
	case sc_dead: {
		SC_DEAD* pack = reinterpret_cast<SC_DEAD*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;

		objEventQue[pack->oid].push(ev);
		if (pack->oid == 20'000)
			objEventQue[GMB_ID].push(ev);
	}break;
	case sc_damaged: {
		SC_DAMAGED* pack = reinterpret_cast<SC_DAMAGED*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;

		objEventQue[pack->oid].push(ev);
	}break;
	case sc_block: {
		SC_BLOCK* pack = reinterpret_cast<SC_BLOCK*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;

		objEventQue[pack->oid].push(ev);
	}break;
	case sc_set_host:
		isHost = true;
		break;
	case sc_berserk: {
		SC_BERSERK* pack = reinterpret_cast<SC_BERSERK*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;

		objEventQue[pack->oid].push(ev);
	}break;
	case sc_move_stop: {
		SC_MOVE_STOP* pack = reinterpret_cast<SC_MOVE_STOP*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;
		ev.pos = pack->pos;

		objEventQue[pack->oid].push(ev);
	}break;
	case sc_sword_on: {
		SC_SWORD_ON * pack = reinterpret_cast<SC_SWORD_ON*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;
		if (pack->oid == my_id)
			wpnType = wpn_sword;

		objEventQue[pack->oid].push(ev);
	}break;
	case sc_sword_off: {
		SC_SWORD_ON * pack = reinterpret_cast<SC_SWORD_ON*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;
		if (pack->oid == my_id)
			wpnType = wpn_none;

		objEventQue[pack->oid].push(ev);
	}break;
	case sc_hammer_on: {
		SC_HAMMER_ON * pack = reinterpret_cast<SC_HAMMER_ON*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;
		if (pack->oid == my_id)
			wpnType = wpn_hammer;

		objEventQue[pack->oid].push(ev);
	}break;
	case sc_hammer_off: {
		SC_HAMMER_ON * pack = reinterpret_cast<SC_HAMMER_ON*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;
		if (pack->oid == my_id)
			wpnType = wpn_none;

		objEventQue[pack->oid].push(ev);
	}break;
	case sc_boss_attack: {
		SC_BOSS_ATTACK* pack = reinterpret_cast<SC_BOSS_ATTACK*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;
		ev.exp = pack->atk_state;
		ev.pos = pack->target;

		objEventQue[pack->oid].push(ev);
	}break;
	case sc_bone_break: {
		SC_BONE_BREAK* pack = reinterpret_cast<SC_BONE_BREAK*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;
		ev.o_type = pack->part;

		objEventQue[pack->oid].push(ev);
	}break;
	case sc_bone_update: {
		SC_BONE_UPDATE* pack = reinterpret_cast<SC_BONE_UPDATE*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;
		ev.o_type = pack->part;
		ev.hp = pack->attacked;

		objEventQue[pack->oid].push(ev);
	}break;
	case sc_set_pos: {
		SC_SET_POS* pack = reinterpret_cast<SC_SET_POS*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;
		ev.pos = pack->pos;

		objEventQue[pack->oid].push(ev);
	}break;
	case sc_set_rotation: {
		SC_SET_ROTATION* pack = reinterpret_cast<SC_SET_ROTATION*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;
		ev.rotation = pack->rot;

		objEventQue[pack->oid].push(ev);
	}break;
	case sc_update_move_state: {
		SC_UPDATE_MOVE_STATE* pack = reinterpret_cast<SC_UPDATE_MOVE_STATE*>(buf);
		GMB_Event ev;
		ev.type = pack->type;
		ev.oid = pack->oid;
		ev.hp = pack->state;

		objEventQue[pack->oid].push(ev);
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

}

int recvn(SOCKET s, char *buf, int len, int flags) {
	int received;
	char *ptr = buf;
	int  left = len;

	recv(s, ptr, 1, flags);
	left = (int)*ptr - 1;
	ptr += 1;
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

