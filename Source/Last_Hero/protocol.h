
#pragma once
#define MAXLEN 10
enum Obj_Type {
	obj_player, obj_monster
};
struct Position {
	float x, y, z;
};
#define MAX_CHAT_LEN 80

enum PACKET_TYPE {
	login_packet, signup_packet, lobby_packet, player_packet, start_packet, init_packet, move_packet, attack_packet, act_packet, skill_packet,
	sc_leave, sc_login, sc_signup, sc_login_fail, sc_login_ok,
	sc_signup_ok, sc_signup_fail, sc_player_move, sc_update_obj, sc_enter_obj,
	sc_leave_obj, cs_chat, sc_chat, cs_attack, cs_guard, sc_attack, sc_guard, cs_fireball, sc_fireball
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

enum OBJ_TYPE {
	OBJ_PLAYER
};

enum P_STATE {
	p_free, p_login,
};

#define MAX_CHAT_LEN 80

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
	char state;
	char usingPart;
};
struct CS_ACT {
	char size;
	char type;
	char detailType;
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

struct SC_PLAYER_MOVE {
	char size;
	char type;
	short uid;
	Position pos;
};

struct SC_OBJECT_ENTER {
	char size;
	char type;
	char o_type;
	char name[MAXLEN];
	short oid;
	Position pos;
};

struct SC_OBJECT_LEAVE {
	char size;
	char type;
	int id;
};

struct CS_CHAT {
	char size;
	char type;
	short uid;
	char chat[MAX_CHAT_LEN];
};

struct SC_CHAT {
	char size;
	char type;
	short uid;
	char chat[MAX_CHAT_LEN];
};

struct CS_ATTACK {
	char size;
	char type;
};

struct CS_GUARD {
	char size;
	char type;
};

struct SC_OBJ_ATTACK {
	char size;
	char type;
	short oid;
};

struct SC_OBJ_GUARD {
	char size;
	char type;
	short oid;
};

struct CS_FIREBALL {
	char size;
	char type;
};

struct SC_FIREBALL {
	char size;
	char type;
	short oid;
};
#pragma pack(pop)