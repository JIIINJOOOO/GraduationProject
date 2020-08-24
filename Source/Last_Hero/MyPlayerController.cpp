// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "Network.h"
extern Network net;
AMyPlayerController::AMyPlayerController() {
	
	PrimaryActorTick.bCanEverTick = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	bEnableMouseOverEvents = true;
	bEnableTouchOverEvents = true;

	isBattle = false;
	isInput = false;
}

void AMyPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AMyPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void AMyPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// 키 하나가 여러 역할을 하는 경우는 나중에 처리
	if (IsInputKeyDown(EKeys::One)) {
		CS_WEAPON_ON pack;
		pack.size = sizeof(CS_WEAPON_ON);
		pack.type = cs_weapon_on;
		net.SendPacket(&pack);
		Sleep(1000);
	}
	if (IsInputKeyDown(EKeys::Two)) {
		CS_WEAPON_ON pack;
		pack.size = sizeof(CS_WEAPON_ON);
		pack.type = cs_weapon_on;
		net.SendPacket(&pack);
		Sleep(1000);
	}
	if (IsInputKeyDown(EKeys::Three)) {
		CS_BERSERK pack;
		pack.size = sizeof(CS_BERSERK);
		pack.type = cs_berserk;
		net.SendPacket(&pack);
		Sleep(1000);
	}
	if (IsInputKeyDown(EKeys::Eight)) {
		CS_FIREBALL pack;
		pack.size = sizeof(CS_FIREBALL);
		pack.type = cs_fireball;
		net.SendPacket(&pack);
		Sleep(1000);
	}
	if (IsInputKeyDown(EKeys::LeftMouseButton)) {
		CS_ATTACK pack;
		pack.size = sizeof(CS_ATTACK);
		pack.type = cs_attack;
		net.SendPacket(&pack);
		Sleep(1000);
	}
	if (IsInputKeyDown(EKeys::RightMouseButton)) {
		CS_GUARD pack;
		pack.size = sizeof(CS_GUARD);
		pack.type = cs_guard;
		net.SendPacket(&pack);
		Sleep(1000);
	}
	if (IsInputKeyDown(EKeys::SpaceBar)) {
		if (isBattle) {
			CS_EVADE pack;
			pack.size = sizeof(CS_EVADE);
			pack.type = cs_evade;
			net.SendPacket(&pack);
		}
		else {
			CS_JUMP pack;
			pack.size = sizeof(CS_JUMP);
			pack.type = cs_jump;
			net.SendPacket(&pack);
		}
	}
	Sleep(1000);
}