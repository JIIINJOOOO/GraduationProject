#pragma once

enum B_STATE {
	B_IDLE, B_CHASE, B_ATTACK, B_DEAD, B_DASH_ATK, B_SWING_ATK, B_STRIKE_ATK, B_THROW_ATK
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