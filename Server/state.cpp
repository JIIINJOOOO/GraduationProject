#include "CMonster.h"
#include <map>
#include <array>
#include <algorithm>
IdleState* IdleState::instance;
ChaseState* ChaseState::instance;
AttackState* AttackState::instance;
ReturnHomeState* ReturnHomeState::instance;
DeadState* DeadState::instance;

extern map<int, CPlayer*> g_player;

// Idle State
IdleState* IdleState::GetInstance() {
	if (instance == nullptr)
		instance = new IdleState;
	return instance;
}

void IdleState::Enter(CMonster* mon) {
	cout << "Monster[" << mon->GetID() << "] is started idle\n";

}

void IdleState::Execute(CMonster* mon) {
	if (mon->GetHealthPoint() <= 0)
		mon->ChangeState(DeadState::GetInstance());

	array<int, MAX_PLAYER> disArray;
	for (int& d : disArray) d = NO_DETECTED;
	
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		int dis = mon->GetDistance(g_player[i]->GetPosition());
		if (dis < CHASE_RANGE) disArray[i] = dis;
	}
	
	int chaseID = NO_DETECTED;
	sort(disArray.begin(), disArray.end());
	for (int i = 0; i<MAX_PLAYER; ++i) 
		if (disArray[i] != NO_DETECTED) {
			chaseID = i;
			break;
		}
	mon->Idle();
	cout << "Idle" << endl;
	if (chaseID == NO_DETECTED) return;
	mon->SetTarget(chaseID);
	if (disArray[chaseID] < ATTACK_RANGE)
		mon->ChangeState(AttackState::GetInstance());
	else if (mon->GetDistance(mon->GetDefPosition()))
		mon->ChangeState(ReturnHomeState::GetInstance());
	else mon->ChangeState(ChaseState::GetInstance());
}

void IdleState::Exit(CMonster* mon) {
	mon->ResetRecoverCool();
}

// Chase State
ChaseState* ChaseState::GetInstance() {
	if (instance == nullptr)
		instance = new ChaseState;
	return instance;
}

void ChaseState::Enter(CMonster* mon) {
	cout << "Monster[" << mon->GetID() << "] is started chase player[" << mon->GetTarget() << "]\n";
	
}

void ChaseState::Execute(CMonster* mon) {
	if (mon->GetHealthPoint() <= 0)
		mon->ChangeState(DeadState::GetInstance());
	int target = mon->GetTarget();
	CPlayer*& tg = g_player[target];
	if (target == NO_DETECTED) return;
	if (tg == NULL) return;

	mon->Chase(*tg);
	cout << "Chase" << endl;
	if (mon->GetDistance(tg->GetPosition()) < ATTACK_RANGE)
		mon->ChangeState(AttackState::GetInstance());
	else if (mon->GetDistance(mon->GetDefPosition()) > ACTIVITY_RANGE)
		mon->ChangeState(ReturnHomeState::GetInstance());
}

void ChaseState::Exit(CMonster* mon) {
	
}

// Attack State
AttackState* AttackState::GetInstance() {
	if (instance == nullptr)
		instance = new AttackState;
	return instance;
}

void AttackState::Enter(CMonster* mon) {
	cout << "Monster[" << mon->GetID() << "] is started attack player[" << mon->GetTarget() << "]\n";
}

void AttackState::Execute(CMonster* mon) {
	if (mon->GetHealthPoint() <= 0)
		mon->ChangeState(DeadState::GetInstance());
	int target = mon->GetTarget();
	if (target == NO_DETECTED) return;
	CPlayer*& tg = g_player[target];
	if (tg == NULL) return;

	mon->Attack(*tg);
	cout << "Attack" << endl;
	if (mon->GetDistance(tg->GetPosition()) > ATTACK_RANGE)
		mon->ChangeState(ChaseState::GetInstance());
	
}

void AttackState::Exit(CMonster*) {

}

// ReturnHome State
ReturnHomeState* ReturnHomeState::GetInstance() {
	if (instance == nullptr)
		instance = new ReturnHomeState;
	return instance;
}

void ReturnHomeState::Enter(CMonster* mon) {
	cout << "Monster[" << mon->GetID() << "] is started return to home\n";
	mon->SetPrevHealthPoint();
}

void ReturnHomeState::Execute(CMonster* mon) {
	if (mon->GetHealthPoint() <= 0)
		mon->ChangeState(DeadState::GetInstance());
	if (mon->GetDistance(mon->GetDefPosition()) < IDLE_RANGE)
		mon->ChangeState(IdleState::GetInstance());
	if (mon->GetHealthPoint() < mon->GetPrevHealthPoint())
		mon->ChangeState(ChaseState::GetInstance());
	cout << "Home" << endl;
}

void ReturnHomeState::Exit(CMonster*) {

}

// Dead State
DeadState* DeadState::GetInstance() {
	if (instance == nullptr)
		instance = new DeadState;
	return instance;
}

void DeadState::Enter(CMonster* mon) {
	cout << "Monster[" << mon->GetID() << "] is Dead\n";
	mon->SetHealthPoint(0);
}

void DeadState::Execute(CMonster* mon) {
	static int tick = 0;
	tick++;
	if (tick == 60 * 10){
		tick = 0;
		mon->ChangeState(IdleState::GetInstance());
	}
	cout << "dead" << endl;
}

void DeadState::Exit(CMonster* mon) {
	cout << "Monster[" << mon->GetID() << "] is Respawn\n";
}
