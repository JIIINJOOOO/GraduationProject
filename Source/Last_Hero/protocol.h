#pragma once
#define MAXLEN 10
enum Obj_Type {
	obj_player, obj_monster
};
struct Position {
	float x, y, z;
};
enum PACKET_TYPE {
	login_packet, signup_packet, lobby_packet, player_packet, start_packet, init_packet, move_packet, attack_packet, act_packet, skill_packet,
	sc_leave, sc_login, sc_signup, sc_login_fail, sc_login_ok,
	sc_signup_ok, sc_signup_fail
};

enum Login_State {
	login_ok, login_error,
	signup_ok, signup_error,
	join_ok, join_error,
	start_ok, start_fail
};

enum Lobby_State {
	create_room, join_room, update_room, out_room, update_player,
	ready, start,
	standby = 10, ingame, full
};

enum P_STATE {
	p_free, p_login,
};

#pragma pack(push, 1)
struct CS_LOGIN {
	char size;
	char type;
	short userIdx;
	char id[MAXLEN];
	char password[MAXLEN];
};
struct SC_LOGIN {
	char size;
	char type;
	char state;
};
struct CS_SIGNUP {
	char size;
	char type;
	char id[MAXLEN];
	char password[MAXLEN];
};
struct SC_SIGNUP {
	char size;
	char type;
	char state;
};


struct SC_LOBBY {
	char size;
	char type;
	char state;
	int idx;
	int users;
};
struct CS_LOBBY {
	char size;
	char type;
	char state;
	int num;
};
struct SC_PLAYER {
	char size;
	char type;
	int idx;
	char id[MAXLEN];
	char state;
};
struct CS_PLAYER {
	char size;
	char type;
	int idx;
	Position pos;
};
struct SC_START {
	char size;
	char type;
	char users[4];
	char state;
};
struct SC_INIT {
	char size;
	char type;
	char id[MAXLEN];
	unsigned short idx;

};
struct CS_MOVE {
	char size;
	char type;
	unsigned short idx;
	Position destination;
};
struct SC_UPDATE_OBJ {
	char size;
	char type;
	Position pos;
	short oid;
};
struct CS_ACT {
	char size;
	char type;
	char detailType;
	unsigned short idx;
};

// New Packets
struct SC_LEAVE {
	char size;
	char type;
	short id;
};

struct SC_LOGIN_FAIL {
	char size;
	char type;
};

struct SC_LOGIN_OK {
	char size;
	char type;
	short uid;
	char name[MAXLEN];
};

struct SC_SIGNUP_OK {
	char size;
	char type;
};

struct SC_SIGNUP_FAIL {
	char size;
	char type;
};
#pragma pack(pop)
