#pragma once

enum B_STATE {
	B_IDLE, B_CHASE, B_ATTACK, B_DEAD, B_DASH_ATK, B_SWING_ATK, B_STRIKE_ATK, B_THROW_ATK, B_STOMP_ATK, B_ICESPEAR_ATK, B_HANDCLAP_ATK, B_PUNCH_ATK, B_DOWN_ATK, B_NORMAL_ATK,
	B_DOWN_ATK_R, B_DOWN_ATK_L, B_DOWN_ATK_SWEEP
};
enum B_ATTACK {
	B_USING_NONE, B_USING_LEFT_HAND, B_USING_RIGHT_HAND, B_USING_LEFT_LEG, B_USING_RIGHT_LEG
};

class Boss;

void ChangeNextState(Boss* b, B_STATE nextState);

class BossState {
public:
	BossState() = default;
	~BossState() = default;
	int state = { B_IDLE };
	int usingPart = {B_USING_NONE};

	virtual void Enter(Boss*) = 0;
	virtual void Execute(Boss*) = 0;
	virtual void Exit(Boss*) = 0;
};

class BossIdle : public BossState {
private:
	static BossIdle* instance;
public:
	static BossIdle* GetInstance();

	virtual void Enter(Boss*);
	virtual void Execute(Boss*);
	virtual void Exit(Boss*);
};

class BossChase : public BossState {
private:
	static BossChase* instance;
public:
	static BossChase* GetInstance();

	virtual void Enter(Boss*);
	virtual void Execute(Boss*);
	virtual void Exit(Boss*);
};

class BossAttack : public BossState {
private:
	static BossAttack* instance;
public:
	static BossAttack* GetInstance();

	virtual void Enter(Boss*);
	virtual void Execute(Boss*);
	virtual void Exit(Boss*);
};

class BossDead : public BossState {
private:
	static BossDead* instance;
public:
	static BossDead* GetInstance();

	virtual void Enter(Boss*);
	virtual void Execute(Boss*);
	virtual void Exit(Boss*);
};


// Attack Pattern
class BossDashAttack : public BossState {
private:
	static BossDashAttack* instance;
public:
	static BossDashAttack* GetInstance();

	virtual void Enter(Boss*);
	virtual void Execute(Boss*);
	virtual void Exit(Boss*);
};

class BossSwingAttack : public BossState {
private:
	static BossSwingAttack* instance;
public:
	static BossSwingAttack* GetInstance();

	virtual void Enter(Boss*);
	virtual void Execute(Boss*);
	virtual void Exit(Boss*);
};

class BossStrikeAttack : public BossState {
private:
	static BossStrikeAttack* instance;
public:
	static BossStrikeAttack* GetInstance();

	virtual void Enter(Boss*);
	virtual void Execute(Boss*);
	virtual void Exit(Boss*);
};

class BossThrowAttack : public BossState {
private:
	static BossThrowAttack* instance;
public:
	static BossThrowAttack* GetInstance();

	virtual void Enter(Boss*);
	virtual void Execute(Boss*);
	virtual void Exit(Boss*);
};


// B_STOMP_ATK, B_ICESPEAR_ATK, B_HANDCLAP_ATK, B_PUNCH_ATK, B_DOWN_ATK, B_NORMAL_ATK

class BossStompAttack : public BossState {
private:
	static BossStompAttack* instance;
public:
	static BossStompAttack* GetInstance();

	virtual void Enter(Boss*);
	virtual void Execute(Boss*);
	virtual void Exit(Boss*);
};

class BossIceSpearAttack : public BossState {
private:
	static BossIceSpearAttack* instance;
public:
	static BossIceSpearAttack* GetInstance();

	virtual void Enter(Boss*);
	virtual void Execute(Boss*);
	virtual void Exit(Boss*);
};

class BossHandClapAttack : public BossState {
private:
	static BossHandClapAttack* instance;
public:
	static BossHandClapAttack* GetInstance();

	virtual void Enter(Boss*);
	virtual void Execute(Boss*);
	virtual void Exit(Boss*);
};

class BossPunchAttack : public BossState {
private:
	static BossPunchAttack* instance;
public:
	static BossPunchAttack* GetInstance();

	virtual void Enter(Boss*);
	virtual void Execute(Boss*);
	virtual void Exit(Boss*);
};

class BossDownAttack : public BossState {
private:
	static BossDownAttack* instance;
public:
	static BossDownAttack* GetInstance();

	virtual void Enter(Boss*);
	virtual void Execute(Boss*);
	virtual void Exit(Boss*);
};