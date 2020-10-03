
#pragma once
#define MAXLEN 10
enum OBJ_TYPE {
	OBJ_PLAYER, OBJ_GOBLIN, OBJ_CYCLOPS, OBJ_BEETLE, OBJ_MINI_GOLEM, OBJ_LAZARD, OBJ_GOLEM
};
struct Position {
	float x, y, z;
};
#define MAX_CHAT_LEN 80

enum PACKET_TYPE {
	cs_login, cs_signup, lobby_packet, player_packet, start_packet, init_packet, move_packet, attack_packet, act_packet, skill_packet,
	sc_leave, sc_login, sc_signup, sc_login_fail, sc_login_ok,
	sc_signup_ok, sc_signup_fail, sc_player_move, sc_update_obj, sc_enter_obj,
	sc_leave_obj, cs_chat, sc_chat, cs_attack, cs_guard, sc_attack, sc_guard, cs_fireball, sc_fireball, sc_set_host, cs_jump, sc_jump,
	cs_evade, cs_weapon_on, cs_weapon_off, sc_evade, sc_weapon_on, sc_weapon_off, cs_berserk, sc_berserk, cs_ready, sc_ready, cs_game_start, sc_game_start, sc_level_up, sc_dead, sc_damaged, sc_get_exp, sc_block, cs_boss_move, sc_fireball_off, sc_fireball_move, cs_hide, sc_hide, cs_hide_off, sc_hide_off, cs_assassin, sc_assassin, cs_move_stop, sc_move_stop, cs_exit, cs_sword_on, cs_sword_off, cs_hammer_on, cs_hammer_off, sc_sword_on, sc_sword_off, sc_hammer_on, sc_hammer_off, cs_npc_move, sc_npc_move, sc_set_npc_target, cs_spawn_npc, cs_boss_attack, sc_boss_attack, cs_boss_bone, sc_bone_break, sc_bone_update, cs_sync_fail, sc_set_pos, sc_set_rotation
};

enum Weapon_Type {
	wpn_none, wpn_sword, wpn_hammer
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

#define MAX_CHAT_LEN 80

#pragma pack(push, 1)
struct CS_LOGIN {
	char size;
	char type;
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
	Position destination;
	Position rotation;
	Position velocity;
};
struct SC_UPDATE_OBJ {
	char size;
	char type;
	Position pos;
	short oid;
	Position rotation;
	Position velocity;
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
	char combo;
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

struct CS_JUMP {
	char size;
	char type;
};

struct SC_JUMP {
	char size;
	char type;
	short oid;
};

struct CS_EVADE {
	char size;
	char type;
};

struct SC_EVADE {
	char size;
	char type;
	short oid;
};

struct CS_WEAPON_ON {
	char size;
	char type;
	char w_type;
};

struct SC_WEAPON_ON {
	char size;
	char type;
	char w_type;
	short oid;
};

struct CS_WEAPON_OFF {
	char size;
	char type;
};

struct SC_WEAPON_OFF {
	char size;
	char type;
	short oid;
};

struct SC_SET_HOST {
	char size;
	char type;
};

struct CS_BERSERK {
	char size;
	char type;
};

struct SC_BERSERK {
	char size;
	char type;
	short oid;
};

struct SC_LEVEL_UP {
	char size;
	char type;
	short oid;
	short level;
	short exp;
	short hp;
	short mp;
};

struct SC_DEAD {
	char size;
	char type;
	short oid;
};

struct SC_DAMAGED {
	char size;
	char type;
	short oid;
	short hp;
};

struct SC_GET_EXP {
	char size;
	char type;
	short exp;
};

struct SC_BLOCK {
	char size;
	char type;
	short oid;
};

struct CS_BOSS_MOVE {
	char size;
	char type;
	Position destination;
	Position rotation;
};

struct SC_FIREBALL_OFF {
	char size;
	char type;
	short owner;
};

struct SC_FIREBALL_MOVE {
	char size;
	char type;
	short owner;
	Position pos;
	Position rotation;
};

struct CS_HIDE {
	char size;
	char type;
};

struct SC_HIDE {
	char size;
	char type;
	short oid;
};

struct CS_HIDE_OFF {
	char size;
	char type;
};

struct SC_HIDE_OFF {
	char size;
	char type;
	short oid;
};

struct CS_ASSASSIN {
	char size;
	char type;
	// short target;
};

struct SC_ASSASSIN {
	char size;
	char type;
	short oid;
};

struct CS_MOVE_STOP {
	char size;
	char type;
};

struct SC_MOVE_STOP {
	char size;
	char type;
	short oid;
	Position pos;
};

struct CS_EXIT {
	char size;
	char type;
};

struct CS_SWORD_ON {
	char size;
	char type;
};

struct CS_SWORD_OFF {
	char size;
	char type;
};

struct CS_HAMMER_ON {
	char size;
	char type;
};

struct CS_HAMMER_OFF {
	char size;
	char type;
};

struct SC_SWORD_ON {
	char size;
	char type;
	short oid;
};

struct SC_SWORD_OFF {
	char size;
	char type;
	short oid;
};

struct SC_HAMMER_ON {
	char size;
	char type;
	short oid;
};

struct SC_HAMMER_OFF {
	char size;
	char type;
	short oid;
};

struct CS_NPC_MOVE {
	char size;
	char type;
	short oid;
	Position pos;
};

struct SC_NPC_MOVE {
	char size;
	char type;
	short oid;
	Position pos;
	Position roatation;
	Position velocity;
};

struct SC_SET_NPC_TARGET {
	char size;
	char type;
	short oid;
	Position pos;
	short target;
};

struct CS_SPAWN_NPC {
	char size;
	char type;
	short oid;
	Position pos;
};

struct CS_BOSS_ATTACK {
	char size;
	char type;
	short oid;
	short atk_num;
};

struct SC_BOSS_ATTACK {
	char size;
	char type;
	short oid;
	short atk_num;
};

struct CS_BOSS_BONE {
	char size;
	char type;
	short oid;
	char boneMap[12];
};

struct SC_BONE_BREAK {
	char size;
	char type;
	short oid;
	short part;
};

struct SC_BONE_UPDATE {
	char size;
	char type;
	short oid;
	char part;
	char attacked;	// ÇÇ°ÝÈ½¼ö
};

struct CS_SYNC_FAIL {
	char size;
	char type;
	short oid;
};

struct SC_SET_POS {
	char size;
	char type;
	short oid;
	Position pos;
};

struct SC_SET_ROTATION {
	char size;
	char type;
	short oid;
	Position rot;
};

#pragma pack(pop)