#include "CMonster.h"
#include <map>
#include <array>
#include <algorithm>
#include "Event.h"
IdleState* IdleState::instance;
ChaseState* ChaseState::instance;
AttackState* AttackState::instance;
ReturnHomeState* ReturnHomeState::instance;
DeadState* DeadState::instance;
extern map<int, CPlayer*> g_player;
extern void send_packet(int uid, void* p);

#define EV_MONSTER 3
// Idle State

IdleState* IdleState::GetInstance() {
	if (instance == nullptr)
		instance = new IdleState;
	return instance;
}


void IdleState::Enter(CMonster* mon) {
	mon->monState = M_IDLE;
	mon->isActive = false;
}

void IdleState::Execute(CMonster* mon) {
	if (mon->GetHealthPoint() <= 0)
		mon->ChangeState(DeadState::GetInstance());
	
	array<int, MAX_PLAYER> disArray;
	for (int& d : disArray)	d = 9'999'999;

	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		if (!g_player[i]->isAlive) continue;
		int dis = mon->GetDistance(g_player[i]->GetPosition());
		if (dis < CHASE_RANGE) disArray[i] = dis;
	}
	
	int chaseID = 0;
	for (int i = 0; i<MAX_PLAYER; ++i) 
		if (disArray[i] != NO_DETECTED) {
			if (disArray[i] < disArray[chaseID])
				chaseID = i;
		}
	
	if (disArray[chaseID] == NO_DETECTED) chaseID = NO_DETECTED;
	if (chaseID != NO_DETECTED && g_player[chaseID] != NULL)
		if (!g_player[chaseID]->isAlive) chaseID = NO_DETECTED;

	mon->Idle();
	if (chaseID == NO_DETECTED) {
		AddTimer(mon->GetID(), EV_MONSTER, high_resolution_clock::now() + 1s, NULL);
		return;
	}
	if (g_player[chaseID] == NULL) {
		AddTimer(mon->GetID(), EV_MONSTER, high_resolution_clock::now() + 1s, NULL);
		return;
	}
	mon->SetTarget(chaseID);
	if (mon->GetDistance(g_player[chaseID]->GetPosition()) < mon->GetAtkRange()) {
		if (mon->GetDistance(g_player[chaseID]->GetPosition()) < 0) return;
		mon->ChangeState(AttackState::GetInstance());
	}
	else if (mon->GetDistance(mon->GetDefPosition()) > ACTIVITY_RANGE)
		mon->ChangeState(ReturnHomeState::GetInstance());
	else if (mon->GetDistance(g_player[chaseID]->GetPosition()) < CHASE_RANGE)
		mon->ChangeState(ChaseState::GetInstance());
	AddTimer(mon->GetID(), EV_MONSTER, high_resolution_clock::now() + 1s, NULL);
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
	cout << "Monster[" << mon->GetID() << "] is started chase " << g_player[mon->GetTarget()]->GetID() << endl;
	mon->monState = M_CHASE;
	
}

void ChaseState::Execute(CMonster* mon) {
	if (mon->GetHealthPoint() <= 0) {
		mon->ChangeState(DeadState::GetInstance());
		AddTimer(mon->GetID(), EV_MONSTER, high_resolution_clock::now() + 1s, NULL);
		return;
	}

	mon->UpdateTarget();
	int target = mon->GetTarget();
	CPlayer*& tg = g_player[target];
	// if (GetDistance(mon->GetPosition(), tg->GetPosition()) > CHASE_RANGE) {
	// 	mon->ChangeState(IdleState::GetInstance());
	// 	AddTimer(mon->GetID(), EV_MONSTER, high_resolution_clock::now() + 1s, NULL);
	// 	return;
	// }
	if (target == NO_DETECTED) return;
	if (tg == NULL) return;

	mon->Chase(*tg);

	if (mon->GetDistance(tg->GetPosition()) < mon->GetAtkRange()) {
		mon->ChangeState(AttackState::GetInstance());
		AddTimer(mon->GetID(), EV_MONSTER, high_resolution_clock::now() + 1s, NULL);
		return;
	}
	// else if (mon->GetDistance(mon->GetDefPosition()) > ACTIVITY_RANGE) {
	// 	mon->ChangeState(ReturnHomeState::GetInstance());
	// 	AddTimer(mon->GetID(), EV_MONSTER, high_resolution_clock::now() + 1s, NULL);
	// 	return;
	// }
	AddTimer(mon->GetID(), EV_MONSTER, high_resolution_clock::now() + 100ms, NULL);	
}

void ChaseState::Exit(CMonster* mon) {
	mon->SetVelocity(Position(0, 0, 0));
	// mon->SetPosition(g_player[mon->GetTarget()]->GetPosition());
	SC_MOVE_STOP pack{sizeof(SC_MOVE_STOP), sc_move_stop, mon->GetID()};
	pack.pos = mon->GetPosition();
	for (int i = 0; i < MAX_PLAYER; ++i) {
		if (g_player[i] == NULL) continue;
		if (!g_player[i]->isAlive) continue;
		// send_packet(i, &pack);
	}
	mon->chaseEndPos = mon->GetPosition();
}

// Attack State
AttackState* AttackState::GetInstance() {
	if (instance == nullptr)
		instance = new AttackState;
	return instance;
}

void AttackState::Enter(CMonster* mon) {
	cout << "Monster[" << mon->GetID() << "] is started attack " << g_player[mon->GetTarget()]->GetID() << endl;
	mon->monState = M_ATTACK;
	// SC_SET_ROTATION pack{ sizeof(SC_SET_ROTATION), sc_set_rotation, mon->GetID() };
	// float y = GetDegree(mon->GetPosition(), g_player[mon->GetTarget()]->GetPosition());
	// pack.rot = { 0, y,0 };
	// for (int i = 0; i < MAX_PLAYER; ++i) {
	// 	if (g_player[i] == NULL) continue;
	// 	send_packet(i, &pack);
	// }
}

void AttackState::Execute(CMonster* mon) {
	if (mon->GetHealthPoint() <= 0) {
		mon->ChangeState(DeadState::GetInstance());
		AddTimer(mon->GetID(), EV_MONSTER, high_resolution_clock::now() + 1s, NULL);
		return;
	}
	int target = mon->GetTarget();
	if (target == NO_DETECTED) {
		mon->ChangeState(IdleState::GetInstance());
		return;
	}
	CPlayer*& tg = g_player[target];
	if (tg == NULL) {
		mon->ChangeState(IdleState::GetInstance());
		return;
	}
	
	if (!tg->isAlive) {
		mon->ChangeState(IdleState::GetInstance());
		return;
	}
	
	mon->Attack(*tg);

	if (mon->GetDistance(tg->GetPosition()) > mon->GetAtkRange()) {
		mon->ChangeState(ChaseState::GetInstance());
		AddTimer(mon->GetID(), EV_MONSTER, high_resolution_clock::now() + 1s, NULL);
		return;
	}
	AddTimer(mon->GetID(), EV_MONSTER, high_resolution_clock::now() + 3s, NULL);
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
	mon->monState = M_RHOME;
}

void ReturnHomeState::Execute(CMonster* mon) {
	if (mon->GetHealthPoint() <= 0)
		mon->ChangeState(DeadState::GetInstance());
	if (mon->GetDistance(mon->GetDefPosition()) < IDLE_RANGE)
		mon->ChangeState(IdleState::GetInstance());
	if (mon->GetHealthPoint() < mon->GetPrevHealthPoint())
		mon->ChangeState(ChaseState::GetInstance());
	AddTimer(mon->GetID(), EV_MONSTER, high_resolution_clock::now() + 1s, NULL);
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
	mon->monState = M_DEAD;
}

void DeadState::Execute(CMonster* mon) {
	// static int tick = 0;
	// tick++;
	// if (tick == 60 * 10){
	// 	tick = 0;
	// 	mon->ChangeState(IdleState::GetInstance());
	// }
	// mon->SetHealthPoint(0);
	// mon->SetPosition(Position(0, 0, 0));
	// mon->ChangeState(IdleState::GetInstance());
	// AddTimer(mon->GetID(), EV_MONSTER, high_resolution_clock::now() + 15s, NULL);
}

void DeadState::Exit(CMonster* mon) {

}
