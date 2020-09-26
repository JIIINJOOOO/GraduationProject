// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMonster.h"
#include "Network.h"
#include "CyclopsAnimInstance.h"
#include "BeetleAnimInstance.h"
#include "MiniGolemAnimInstance.h"
#include "LazardAnimInstance.h"


extern Network net;

// Sets default values
AMyMonster::AMyMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// static ConstructorHelpers::FClassFinder<UObject> MON_AICONTROLLER(TEXT("/Game/Game/BluePrints/mini_golem/Ai_Monster_minigolem.Ai_Monster_minigolem"));


	// if (MON_AICONTROLLER.Succeeded())
	// {
	// 	AIControllerClass = MON_AICONTROLLER.Class;
	// 	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	// }

	PrimaryActorTick.bCanEverTick = true;
	speed = 200.f;
	velocity = { 0,0,0 };
	isDead = false;
	id = -1;
	type = -1;
	deadCnt = 0;
	isMoving = false;
}

// Called when the game starts or when spawned
void AMyMonster::BeginPlay()
{
	Super::BeginPlay();
	MonPos = GetActorLocation();
	SpawnDefaultController();
}

// Called to bind functionality to input
void AMyMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

// Called every frame
void AMyMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MonPos = GetActorLocation();
	
	if (isMoving) AddMovementInput(velocity, speed, true);

	if (type == OBJ_CYCLOPS)
		CyclopsUpdate();
	else if (type == OBJ_BEETLE)
		BeetleUpdate();
	else if (type == OBJ_MINI_GOLEM)
		MiniGolemUpdate();
	else if (type == OBJ_LAZARD)
		LazardUpdate();
}

void AMyMonster::CyclopsUpdate() {
	net.eventLock.lock();
	if (net.eventQue.empty()) {
		net.eventLock.unlock();
		return;
	}
	auto ev = net.eventQue.front();
	net.eventLock.unlock();
	if (ev.oid < NPC_ID_START) return;
	if (ev.oid != id) return;

	switch (ev.type) {
	case sc_update_obj:
		MonPos = { ev.pos.x, ev.pos.y, MonPos.Z };
		rotation = { ev.rotation.x, ev.rotation.y, ev.rotation.z };
		SetActorLocationAndRotation(MonPos, rotation, false, 0, ETeleportType::TeleportPhysics);
		isMoving = true;
		net.PopEvent();
		break;
	case sc_attack: {
		auto animInst = Cast<UCyclopsAnimInstance>(GetMesh()->GetAnimInstance());
		if (animInst != nullptr) {
			if (ev.mp == 0) animInst->Attack1();
			if (ev.mp == 1) animInst->Attack2();
			if (ev.mp == 2) animInst->Attack3();
		}
		isMoving = false;
		net.PopEvent();
	}break;
	case sc_damaged: {
		auto animInst = Cast<UCyclopsAnimInstance>(GetMesh()->GetAnimInstance());
		if (animInst != nullptr) animInst->Hitreaction();
		isMoving = false;
		net.PopEvent();
	}break;
	case sc_dead: {
		// isDead = true;
		auto animInst = Cast<UCyclopsAnimInstance>(GetMesh()->GetAnimInstance());
		if (animInst != nullptr) animInst->Hitreaction();
		isMoving = false;
		net.PopEvent();
	}break;
	default:
		isMoving = false;
		net.PopEvent();
		break;
	}
}

void AMyMonster::BeetleUpdate() {
	net.eventLock.lock();
	if (net.eventQue.empty()) {
		net.eventLock.unlock();
		return;
	}
	auto ev = net.eventQue.front();
	net.eventLock.unlock();
	if (ev.oid < NPC_ID_START) return;
	if (ev.oid != id) return;

	switch (ev.type) {
	case sc_update_obj:
		MonPos = { ev.pos.x, ev.pos.y, MonPos.Z };
		rotation = { ev.rotation.x, ev.rotation.y, ev.rotation.z };
		velocity = { ev.velocity.x, ev.velocity.y, ev.velocity.z };
		SetActorLocationAndRotation(MonPos, rotation, false, 0, ETeleportType::TeleportPhysics);
		isMoving = true;
		net.PopEvent();
		break;
	case sc_attack: {
		auto animInst = Cast<UBeetleAnimInstance>(GetMesh()->GetAnimInstance());
		if (animInst != nullptr) {
			if (ev.mp == 0) animInst->Attack1();
			if (ev.mp == 1) animInst->Attack2();
			if (ev.mp == 2) animInst->Attack3();
		}
		isMoving = false;
		net.PopEvent();
	}break;
	case sc_damaged: {
		auto animInst = Cast<UBeetleAnimInstance>(GetMesh()->GetAnimInstance());
		if (animInst != nullptr) animInst->Hitreaction();
		isMoving = false;
		net.PopEvent();
	}break;
	case sc_dead: {
		// isDead = true;
		auto animInst = Cast<UBeetleAnimInstance>(GetMesh()->GetAnimInstance());
		if (animInst != nullptr) animInst->Hitreaction();
		isMoving = false;
		net.PopEvent();
	}break;
	default:
		isMoving = false;
		net.PopEvent();
		break;
	}
}

void AMyMonster::MiniGolemUpdate() {
	net.eventLock.lock();
	if (net.eventQue.empty()) {
		net.eventLock.unlock();
		return;
	}
	auto ev = net.eventQue.front();
	net.eventLock.unlock();
	if (ev.oid < NPC_ID_START) return;
	if (ev.oid != id) return;

	switch (ev.type) {
	case sc_update_obj:
		MonPos = { ev.pos.x, ev.pos.y, MonPos.Z };
		rotation = { ev.rotation.x, ev.rotation.y, ev.rotation.z };
		velocity = { ev.velocity.x, ev.velocity.y, ev.velocity.z };
		SetActorLocationAndRotation(MonPos, rotation, false, 0, ETeleportType::TeleportPhysics);
		isMoving = true;
		net.PopEvent();
		break;
	case sc_attack: {
		auto animInst = Cast<UMiniGolemAnimInstance>(GetMesh()->GetAnimInstance());
		if (animInst != nullptr) {
			if (ev.mp == 0) animInst->Attack1();
			if (ev.mp == 1) animInst->Attack2();
			if (ev.mp == 2) animInst->Attack3();
		}
		isMoving = false;
		net.PopEvent();
	}break;
	case sc_damaged: {
		auto animInst = Cast<UMiniGolemAnimInstance>(GetMesh()->GetAnimInstance());
		if (animInst != nullptr) animInst->Hitreaction();
		isMoving = false;
		net.PopEvent();
	}break;
	case sc_dead: {
		// isDead = true;
		auto animInst = Cast<UMiniGolemAnimInstance>(GetMesh()->GetAnimInstance());
		if (animInst != nullptr) animInst->Hitreaction();
		isMoving = false;
		net.PopEvent();
	}break;
	default:
		isMoving = false;
		net.PopEvent();
		break;
	}
}

void AMyMonster::LazardUpdate() {
	net.eventLock.lock();
	if (net.eventQue.empty()) {
		net.eventLock.unlock();
		return;
	}
	auto ev = net.eventQue.front();
	net.eventLock.unlock();
	if (ev.oid < NPC_ID_START) return;
	if (ev.oid != id) return;

	switch (ev.type) {
	case sc_update_obj:
		MonPos = { ev.pos.x, ev.pos.y, MonPos.Z };
		rotation = { ev.rotation.x, ev.rotation.y, ev.rotation.z };
		velocity = { ev.velocity.x, ev.velocity.y, ev.velocity.z };
		SetActorLocationAndRotation(MonPos, rotation, false, 0, ETeleportType::TeleportPhysics);
		isMoving = true;
		net.PopEvent();
		break;
	case sc_attack: {
		auto animInst = Cast<ULazardAnimInstance>(GetMesh()->GetAnimInstance());
		if (animInst != nullptr) {
			if (ev.mp == 0) animInst->Attack1();
			if (ev.mp == 1) animInst->Attack2();
			if (ev.mp == 2) animInst->Attack3();
		}
		isMoving = false;
		net.PopEvent();
	}break;
	case sc_damaged: {
		auto animInst = Cast<ULazardAnimInstance>(GetMesh()->GetAnimInstance());
		if (animInst != nullptr) animInst->Hitreaction();
		isMoving = false;
		net.PopEvent();
	}break;
	case sc_dead: {
		// isDead = true;
		auto animInst = Cast<ULazardAnimInstance>(GetMesh()->GetAnimInstance());
		if (animInst != nullptr) animInst->Hitreaction();
		isMoving = false;
		net.PopEvent();
	}break;
	default:
		isMoving = false;
		net.PopEvent();
		break;
	}
}

bool AMyMonster::IsCyclops() {
	return (CYCLOPS_ID <= id && id < BEETLE_ID);
}

bool AMyMonster::IsBeetle() {
	return (BEETLE_ID <= id && id < MINI_GOLEM_ID);
}

bool AMyMonster::IsMiniGolem() {
	return MINI_GOLEM_ID <= id;
}

void AMyMonster::SetID(const int& new_id) {
	id = new_id;
}

void AMyMonster::SetType(const int& mon_type) {
	type = mon_type;
}