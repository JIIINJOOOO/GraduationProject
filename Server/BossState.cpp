#include "BossState.h"
#include "Boss.h"

BossIdle* BossIdle::instance;
BossChase* BossChase::instance;
BossAttack* BossAttack::instance;
BossDead* BossDead::instance;
BossDashAttack* BossDashAttack::instance;
BossSwingAttack* BossSwingAttack::instance;
BossStrikeAttack* BossStrikeAttack::instance;
BossThrowAttack* BossThrowAttack::instance;

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
	usingPart = B_USING_NONE;
	if (nextState != B_IDLE)
		ChangeNextState(b, nextState);
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
	if (b_destroy.leftLeg || b_destroy.rightLeg) {
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
	// 여기서 어떤 공격할지 골라서 반환해주고 다음 타임에서 그 행동을 한다
	// 이름은 ATTACK이지만 사실상 공격대기 상태
	B_STATE nextState = b->Attack();
	usingPart = B_USING_NONE;	//
	if (nextState != B_ATTACK)
		ChangeNextState(b, nextState);
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
	// 초기화 및 리스폰 진행
	usingPart = B_USING_NONE;
	// 1cout<<"안지나가?"<<endl;
	b->Respawn();
	b->ChangeState(BossIdle::GetInstance());
}

void BossDead::Exit(Boss*) {
	cout << "Boss is Respawn!" << endl;

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
	auto b_destroy = b->GetIsDestroy();
	usingPart = B_USING_NONE;	// 발을 사용하지만 한쪽만 나가리되도 못쓰니까 일단 None으로 설정한다
	if (b_destroy.leftLeg || b_destroy.rightLeg)
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
	auto b_destroy = b->GetIsDestroy();
	if (b_destroy.leftHand && b_destroy.rightHand) {
		b->ChangeState(BossAttack::GetInstance());
		return;
	}
	// 어느쪽 손을 휘두를지 결정하고 클라이언트에서 설정한 값에 따른 애니메이션을 재생한다
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
	auto b_destroy = b->GetIsDestroy();
	if (b_destroy.leftHand && b_destroy.rightHand) {
		b->ChangeState(BossAttack::GetInstance());
		return;
	}
	// 어느쪽 손으로 내려칠지 결정하고 클라이언트에서 설정한 값에 따른 애니메이션을 재생한다
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
	auto b_destroy = b->GetIsDestroy();
	if (b_destroy.leftHand && b_destroy.rightHand) {
		b->ChangeState(BossAttack::GetInstance());
		return;
	}
	// 어느쪽 손으로 던질지 결정하고 클라이언트에서 설정한 값에 따른 애니메이션을 재생한다
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