#include "BossState.h"
#include "Boss.h"
#include "CPlayer.h"
#include "Event.h"
#include <map>
#include <random>
// down 1 -> L hand
// down 2 -> R hand
// down sweep -> L hand

extern map<int, CPlayer*> g_player;
BossIdle* BossIdle::instance;
BossChase* BossChase::instance;
BossAttack* BossAttack::instance;
BossDead* BossDead::instance;
BossDashAttack* BossDashAttack::instance;
BossSwingAttack* BossSwingAttack::instance;
BossStrikeAttack* BossStrikeAttack::instance;
BossThrowAttack* BossThrowAttack::instance;
BossIceSpearAttack* BossIceSpearAttack::instance;
BossPunchAttack* BossPunchAttack::instance;
BossDownAttack* BossDownAttack::instance;
BossStompAttack* BossStompAttack::instance;
BossHandClapAttack* BossHandClapAttack::instance;


extern void send_packet(int uid, void* p);
extern float GetDegree(Position p1, Position p2);

extern default_random_engine dre;
extern uniform_int_distribution<> uid;

void ChangeNextState(Boss* b, B_STATE nextState) {
	switch (nextState) {
	case B_IDLE:
		b->ChangeState(BossIdle::GetInstance());
		break;
	case B_CHASE:
		b->ChangeState(BossChase::GetInstance());
		break;
	case B_ATTACK:
		b->ChangeState(BossAttack::GetInstance());
		break;
	case B_DEAD:
		b->ChangeState(BossDead::GetInstance());
		break;
	case B_DASH_ATK:
		b->ChangeState(BossDashAttack::GetInstance());
		break;
	case B_SWING_ATK:
		b->ChangeState(BossSwingAttack::GetInstance());
		break;
	case B_STRIKE_ATK:
		b->ChangeState(BossStrikeAttack::GetInstance());
		break;
	case B_THROW_ATK:
		b->ChangeState(BossThrowAttack::GetInstance());
		break;
	case B_STOMP_ATK:
		b->ChangeState(BossStompAttack::GetInstance());
		break;
	case B_ICESPEAR_ATK:
		b->ChangeState(BossIceSpearAttack::GetInstance());
		break;
	case B_HANDCLAP_ATK:
		b->ChangeState(BossHandClapAttack::GetInstance());
		break;
	case B_PUNCH_ATK:
		b->ChangeState(BossPunchAttack::GetInstance());
		break;
	case B_DOWN_ATK:
		b->ChangeState(BossDownAttack::GetInstance());
		break;
	default:
		b->ChangeState(BossDead::GetInstance());
		cout << "No defined State" << endl;
		break;
	}
}

// Idle
BossIdle* BossIdle::GetInstance() {
	if (instance == nullptr)
		instance = new BossIdle;
	return instance;
}

void BossIdle::Enter(Boss*){
	state = B_IDLE;
}

void BossIdle::Execute(Boss* b) {
	B_STATE nextState = b->Idle();
	if (nextState != B_IDLE) ChangeNextState(b, nextState);
	AddTimer(b->GetID(), 4, high_resolution_clock::now() + 1s, b->GetTarget());
}

void BossIdle::Exit(Boss*) {

}


// Chase
BossChase* BossChase::GetInstance() {
	if (instance == nullptr)
		instance = new BossChase;
	return instance;
}

void BossChase::Enter(Boss* b) {
	state = B_CHASE;
}

void BossChase::Execute(Boss* b) {
	auto b_destroy = b->GetIsDestroy();
	usingPart = B_USING_NONE;
	if (b_destroy.leftFoot || b_destroy.rightFoot) {
		b->ChangeState(BossIdle::GetInstance());
		return;
	}
	auto nextState = b->Chase();
	if (nextState != B_CHASE) 
		ChangeNextState(b, nextState);
}

void BossChase::Exit(Boss*) {

}


// Attack
BossAttack* BossAttack::GetInstance() {
	if (instance == nullptr)
		instance = new BossAttack;
	return instance;
}

void BossAttack::Enter(Boss*) {
	state = B_ATTACK;

}

void BossAttack::Execute(Boss* b) {
	// ���⼭ � �������� ��� ��ȯ���ְ� ���� Ÿ�ӿ��� �� �ൿ�� �Ѵ�
	// �̸��� ATTACK������ ��ǻ� ���ݴ�� ����
	B_STATE nextAttack = b->Attack();
	if (nextAttack == B_ATTACK) {
		AddTimer(b->GetID(), 4, high_resolution_clock::now() + 1s, b->GetTarget());
		return;
	}
	if (nextAttack == B_NORMAL_ATK) {
		cout << "Boss use Normal-Attack!\n";
		SC_SET_ROTATION rpack{ sizeof(SC_SET_ROTATION), sc_set_rotation, b->GetID() };
		auto ydegree = GetDegree(b->GetPosition(), g_player[b->GetTarget()]->GetPosition());
		rpack.rot = { 0,ydegree,0 };
		for (int i = 0; i < MAX_PLAYER; ++i) {
			if (g_player[i] == NULL) continue;
			send_packet(i, &rpack);
		}

		SC_BOSS_ATTACK pack{ sizeof(SC_BOSS_ATTACK), sc_boss_attack, b->GetID() };
		pack.atk_state = B_NORMAL_ATK;
		pack.target = g_player[b->GetTarget()]->GetPosition();
		for (int i = 0; i < MAX_PLAYER; ++i) {
			if (g_player[i] == NULL) continue;
			send_packet(i, &pack);
		}
	}
	else ChangeNextState(b, nextAttack);
	AddTimer(b->GetID(), 4, high_resolution_clock::now() + 1s, b->GetTarget());
	// SC_BOSS_ATTACK pack{ sizeof(SC_BOSS_ATTACK), sc_boss_attack, nextAttack };
	// pack.target = g_player[b->GetTarget()]->GetPosition();
	// for (int i = 0; i < MAX_PLAYER; ++i) {
	// 	if (g_player[i] == NULL) continue;
	// 	send_packet(i, &pack);
	// }
	// usingPart = B_USING_NONE;	//
	// if (nextState != B_ATTACK)
	// 	ChangeNextState(b, nextState);
}

void BossAttack::Exit(Boss*) {

}


// Dead
BossDead* BossDead::GetInstance() {
	if (instance == nullptr)
		instance = new BossDead;
	return instance;
}

void BossDead::Enter(Boss* b) {
	cout << "Boss is Dead" << endl;
	state = B_DEAD;
}

void BossDead::Execute(Boss* b) {
	// �ʱ�ȭ �� ������ ����
	usingPart = B_USING_NONE;
	// 1cout<<"��������?"<<endl;
	// b->ChangeState(BossIdle::GetInstance());
}

void BossDead::Exit(Boss*) {
	// cout << "Boss is Respawn!" << endl;

}

// DashAttack
BossDashAttack* BossDashAttack::GetInstance() {
	if (instance == nullptr)
		instance = new BossDashAttack;
	return instance;
}

void BossDashAttack::Enter(Boss*) {
	state = B_DASH_ATK;
}

void BossDashAttack::Execute(Boss* b) {
	if (g_player[b->GetTarget()] == NULL || b->IsLegBroken()) {
		b->ChangeState(BossAttack::GetInstance());
		return;
	}

	SC_SET_ROTATION rpack{ sizeof(SC_SET_ROTATION), sc_set_rotation, b->GetID() };
	auto ydegree = GetDegree(b->GetPosition(), g_player[b->GetTarget()]->GetPosition());
	rpack.rot = { 0,ydegree,0 };
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &rpack);
	}

	cout << "Boss use Dash-Attack!\n";
	SC_BOSS_ATTACK pack{ sizeof(SC_BOSS_ATTACK), sc_boss_attack, b->GetID() };
	pack.atk_state = B_DASH_ATK;
	pack.target = g_player[b->GetTarget()]->GetPosition();
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &pack);
	}
	b->ChangeState(BossAttack::GetInstance());
	AddTimer(b->GetID(), 4, high_resolution_clock::now() + 5s, b->GetTarget());
	return;
	auto b_destroy = b->GetIsDestroy();
	usingPart = B_USING_NONE;	// ���� ��������� ���ʸ� �������ǵ� �����ϱ� �ϴ� None���� �����Ѵ�
	if (b_destroy.leftFoot || b_destroy.rightFoot)
		b->ChangeState(BossIdle::GetInstance());
}

void BossDashAttack::Exit(Boss*) {

}


// SwingAttack
BossSwingAttack* BossSwingAttack::GetInstance() {
	if (instance == nullptr)
		instance = new BossSwingAttack;
	return instance;
}

void BossSwingAttack::Enter(Boss*) {
	state = B_SWING_ATK;
}

void BossSwingAttack::Execute(Boss* b) {
	if (g_player[b->GetTarget()] == NULL || b->IsLegBroken()) {
		b->ChangeState(BossAttack::GetInstance());
		return;
	}

	SC_SET_ROTATION rpack{ sizeof(SC_SET_ROTATION), sc_set_rotation, b->GetID() };
	auto ydegree = GetDegree(b->GetPosition(), g_player[b->GetTarget()]->GetPosition());
	rpack.rot = { 0,ydegree,0 };
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &rpack);
	}

	cout << "Boss use Swing-Attack!\n";
	SC_BOSS_ATTACK pack{ sizeof(SC_BOSS_ATTACK), sc_boss_attack, b->GetID() };
	pack.atk_state = B_SWING_ATK;
	pack.target = g_player[b->GetTarget()]->GetPosition();
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &pack);
	}
	b->ChangeState(BossAttack::GetInstance());
	AddTimer(b->GetID(), 4, high_resolution_clock::now() + 3s, b->GetTarget());
	return;
	auto b_destroy = b->GetIsDestroy();
	if (b_destroy.leftHand && b_destroy.rightHand) {
		b->ChangeState(BossAttack::GetInstance());
		return;
	}
	// ����� ���� �ֵθ��� �����ϰ� Ŭ���̾�Ʈ���� ������ ���� ���� �ִϸ��̼��� ����Ѵ�
	else if (b_destroy.leftHand && !b_destroy.rightHand)
		usingPart = B_USING_RIGHT_HAND;
	else if (!b_destroy.leftHand && b_destroy.rightHand)
		usingPart = B_USING_LEFT_HAND;
	else{
		if (rand() % 2 == 0)
			usingPart = B_USING_LEFT_HAND;
		else usingPart = B_USING_RIGHT_HAND;
	}
}

void BossSwingAttack::Exit(Boss*) {

}


// StrikeAttack
BossStrikeAttack* BossStrikeAttack::GetInstance() {
	if (instance == nullptr)
		instance = new BossStrikeAttack;
	return instance;
}

void BossStrikeAttack::Enter(Boss*) {
	state = B_STRIKE_ATK;
}

void BossStrikeAttack::Execute(Boss* b) {
	if (g_player[b->GetTarget()] == NULL || b->IsLegBroken()) {
		b->ChangeState(BossAttack::GetInstance());
		return;
	}

	SC_SET_ROTATION rpack{ sizeof(SC_SET_ROTATION), sc_set_rotation, b->GetID() };
	auto ydegree = GetDegree(b->GetPosition(), g_player[b->GetTarget()]->GetPosition());
	rpack.rot = { 0,ydegree,0 };
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &rpack);
	}

	cout << "Boss use Strike-Attack!\n";
	SC_BOSS_ATTACK pack{ sizeof(SC_BOSS_ATTACK), sc_boss_attack, b->GetID() };
	pack.atk_state = B_STRIKE_ATK;
	pack.target = g_player[b->GetTarget()]->GetPosition();
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &pack);
	}
	b->ChangeState(BossAttack::GetInstance());
	AddTimer(b->GetID(), 4, high_resolution_clock::now() + 3s, b->GetTarget());
	return; 
	auto b_destroy = b->GetIsDestroy();
	if (b_destroy.leftHand && b_destroy.rightHand) {
		b->ChangeState(BossAttack::GetInstance());
		return;
	}
	// ����� ������ ����ĥ�� �����ϰ� Ŭ���̾�Ʈ���� ������ ���� ���� �ִϸ��̼��� ����Ѵ�
	else if (b_destroy.leftHand && !b_destroy.rightHand)
		usingPart = B_USING_RIGHT_HAND;
	else if (!b_destroy.leftHand && b_destroy.rightHand)
		usingPart = B_USING_LEFT_HAND;
	else {
		if (rand() % 2 == 0)
			usingPart = B_USING_LEFT_HAND;
		else usingPart = B_USING_RIGHT_HAND;
	}
}

void BossStrikeAttack::Exit(Boss*) {

}


// ThrowAttack
BossThrowAttack* BossThrowAttack::GetInstance() {
	if (instance == nullptr)
		instance = new BossThrowAttack;
	return instance;
}

void BossThrowAttack::Enter(Boss*) {
	state = B_THROW_ATK;
}

void BossThrowAttack::Execute(Boss* b) {
	if (g_player[b->GetTarget()] == NULL || b->IsLegBroken()) {
		b->ChangeState(BossAttack::GetInstance());
		return;
	}

	SC_SET_ROTATION rpack{ sizeof(SC_SET_ROTATION), sc_set_rotation, b->GetID() };
	auto ydegree = GetDegree(b->GetPosition(), g_player[b->GetTarget()]->GetPosition());
	rpack.rot = { 0,ydegree,0 };
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &rpack);
	}

	cout << "Boss use Throw-Attack!\n";
	SC_BOSS_ATTACK pack{ sizeof(SC_BOSS_ATTACK), sc_boss_attack, b->GetID() };
	pack.atk_state = B_THROW_ATK;
	pack.target = g_player[b->GetTarget()]->GetPosition();
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &pack);
	}
	b->ChangeState(BossAttack::GetInstance());
	AddTimer(b->GetID(), 4, high_resolution_clock::now() + 5s, b->GetTarget());
	return;

	// ������
	auto b_destroy = b->GetIsDestroy();
	if (b_destroy.leftHand && b_destroy.rightHand) {
		b->ChangeState(BossAttack::GetInstance());
		return;
	}
	// ����� ������ ������ �����ϰ� Ŭ���̾�Ʈ���� ������ ���� ���� �ִϸ��̼��� ����Ѵ�
	else if (b_destroy.leftHand && !b_destroy.rightHand)
		usingPart = B_USING_RIGHT_HAND;
	else if (!b_destroy.leftHand && b_destroy.rightHand)
		usingPart = B_USING_LEFT_HAND;
	else {
		if (rand() % 2 == 0)
			usingPart = B_USING_LEFT_HAND;
		else usingPart = B_USING_RIGHT_HAND;
	}
}

void BossThrowAttack::Exit(Boss*) {

}


// StompAttack
BossStompAttack* BossStompAttack::GetInstance() {
	if (instance == nullptr)
		instance = new BossStompAttack;
	return instance;
}

void BossStompAttack::Enter(Boss*) {

}

void BossStompAttack::Execute(Boss* b) {
	if (g_player[b->GetTarget()] == NULL || b->IsLegBroken()) {
		b->ChangeState(BossAttack::GetInstance());
		return;
	}

	SC_SET_ROTATION rpack{ sizeof(SC_SET_ROTATION), sc_set_rotation, b->GetID() };
	auto ydegree = GetDegree(b->GetPosition(), g_player[b->GetTarget()]->GetPosition());
	rpack.rot = { 0,ydegree,0 };
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &rpack);
	}

	cout << "Boss use Stomp-Attack!\n";
	SC_BOSS_ATTACK pack{ sizeof(SC_BOSS_ATTACK), sc_boss_attack, b->GetID() };
	pack.atk_state = B_STOMP_ATK;
	pack.target = g_player[b->GetTarget()]->GetPosition();
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &pack);
	}
	b->ChangeState(BossAttack::GetInstance());
	AddTimer(b->GetID(), 4, high_resolution_clock::now() + 5s, b->GetTarget());
}

void BossStompAttack::Exit(Boss*) {

}


// IceSpear
BossIceSpearAttack* BossIceSpearAttack::GetInstance() {
	if (instance == nullptr)
		instance = new BossIceSpearAttack;
	return instance;
}

void BossIceSpearAttack::Enter(Boss*) {

}

void BossIceSpearAttack::Execute(Boss* b) {
	if (g_player[b->GetTarget()] == NULL || b->IsLegBroken()) {
		b->ChangeState(BossAttack::GetInstance());
		return;
	}

	SC_SET_ROTATION rpack{ sizeof(SC_SET_ROTATION), sc_set_rotation, b->GetID() };
	auto ydegree = GetDegree(b->GetPosition(), g_player[b->GetTarget()]->GetPosition());
	rpack.rot = { 0,ydegree,0 };
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &rpack);
	}

	cout << "Boss use IceSpear-Attack!\n";
	SC_BOSS_ATTACK pack{ sizeof(SC_BOSS_ATTACK), sc_boss_attack, b->GetID() };
	pack.atk_state = B_ICESPEAR_ATK;
	pack.target = g_player[b->GetTarget()]->GetPosition();
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &pack);
	}
	b->ChangeState(BossAttack::GetInstance());
	AddTimer(b->GetID(), 4, high_resolution_clock::now() + 5s, b->GetTarget());
}

void BossIceSpearAttack::Exit(Boss*) {

}


// HandClap ����~
BossHandClapAttack* BossHandClapAttack::GetInstance() {
	if (instance == nullptr)
		instance = new BossHandClapAttack;
	return instance;
}

void BossHandClapAttack::Enter(Boss*) {

}

void BossHandClapAttack::Execute(Boss* b) {
	if (g_player[b->GetTarget()] == NULL || b->IsLegBroken()) {
		b->ChangeState(BossAttack::GetInstance());
		return;
	}

	SC_SET_ROTATION rpack{ sizeof(SC_SET_ROTATION), sc_set_rotation, b->GetID() };
	auto ydegree = GetDegree(b->GetPosition(), g_player[b->GetTarget()]->GetPosition());
	rpack.rot = { 0,ydegree,0 };
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &rpack);
	}

	cout << "Boss use HandClap-Attack!\n";
	SC_BOSS_ATTACK pack{ sizeof(SC_BOSS_ATTACK), sc_boss_attack, b->GetID() };
	pack.atk_state = B_HANDCLAP_ATK;
	pack.target = g_player[b->GetTarget()]->GetPosition();
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &pack);
	}
	b->ChangeState(BossAttack::GetInstance());
	AddTimer(b->GetID(), 4, high_resolution_clock::now() + 5s, b->GetTarget());
}

void BossHandClapAttack::Exit(Boss*) {

}


// Punch
BossPunchAttack* BossPunchAttack::GetInstance() {
	if (instance == nullptr)
		instance = new BossPunchAttack;
	return instance;
}

void BossPunchAttack::Enter(Boss*) {

}

void BossPunchAttack::Execute(Boss* b) {
	if (g_player[b->GetTarget()] == NULL || b->IsLegBroken()) {
		b->ChangeState(BossAttack::GetInstance());
		return;
	}

	SC_SET_ROTATION rpack{ sizeof(SC_SET_ROTATION), sc_set_rotation, b->GetID() };
	auto ydegree = GetDegree(b->GetPosition(), g_player[b->GetTarget()]->GetPosition());
	rpack.rot = { 0,ydegree,0 };
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &rpack);
	}

	cout << "Boss use Punch-Attack!\n";
	SC_BOSS_ATTACK pack{ sizeof(SC_BOSS_ATTACK), sc_boss_attack, b->GetID() };
	pack.atk_state = B_PUNCH_ATK;
	pack.target = g_player[b->GetTarget()]->GetPosition();
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &pack);
	}
	b->ChangeState(BossAttack::GetInstance());
	AddTimer(b->GetID(), 4, high_resolution_clock::now() + 5s, b->GetTarget());
}

void BossPunchAttack::Exit(Boss*) {

}


// Down
BossDownAttack* BossDownAttack::GetInstance() {
	if (instance == nullptr)
		instance = new BossDownAttack;
	return instance;
}

void BossDownAttack::Enter(Boss*) {

}

void BossDownAttack::Execute(Boss* b) {
	if (g_player[b->GetTarget()] == NULL || b->IsLegBroken() == false) {
		b->ChangeState(BossAttack::GetInstance());
		return;
	}
	SC_SET_ROTATION rpack{ sizeof(SC_SET_ROTATION), sc_set_rotation, b->GetID() };
	auto ydegree = GetDegree(b->GetPosition(), g_player[b->GetTarget()]->GetPosition());
	rpack.rot = { 0,ydegree,0 };
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &rpack);
	}
	
	int atk_state = 0;
	if (b->IsLHandBroken()) {
		if (b->IsRHandBroken()) return;
		atk_state = B_DOWN_ATK_R;
	}
	else if (b->IsRHandBroken()) {
		if (b->IsLHandBroken()) return;
		switch (uid(dre) % 2) {
		case 0:
			atk_state = B_DOWN_ATK_L;
			break;
		case 1:
			atk_state = B_DOWN_ATK_SWEEP;
			break;
		}
	}
	else {
		// �� ����
		switch (uid(dre) % 3){
		case 0:	
			atk_state = B_DOWN_ATK_L;
			break;
		case 1: 
			atk_state = B_DOWN_ATK_R;
			break;
		case 2:
			atk_state = B_DOWN_ATK_SWEEP;
			break;
		}
	}
	if (atk_state == 0) return;
	cout << "Boss use Down-Attack!\n";
	SC_BOSS_ATTACK pack{ sizeof(SC_BOSS_ATTACK), sc_boss_attack, b->GetID() };
	pack.atk_state = atk_state;
	pack.target = g_player[b->GetTarget()]->GetPosition();
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		send_packet(i, &pack);
	}
	b->ChangeState(BossAttack::GetInstance());
	AddTimer(b->GetID(), 4, high_resolution_clock::now() + 5s, b->GetTarget());
}

void BossDownAttack::Exit(Boss*) {

}

