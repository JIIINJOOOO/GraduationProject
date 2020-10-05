// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMonster.h"
#include "CyclopsAnimInstance.h"
#include "BeetleAnimInstance.h"
#include "MiniGolemAnimInstance.h"
#include "LazardAnimInstance.h"


extern Network net;

// Sets default values
AMyMonster::AMyMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// static ConstructorHelpers::FClassFinder<UObject> MON_AICONTROLLER(TEXT("/Game/Game/BluePrints/mini_golem/Ai_Monster_minigolem.Ai_Monster_minigolem_C"));


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
	SpawnDefaultController();
	MonPos = GetActorLocation();
	netPos = MonPos;
	SetActorRotation(FRotator(0, 180, 0));
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
	netPos.Z = MonPos.Z;

	if (type == -1) return;
	if (id == -1) return;

	if (isDead) {
		if (deathTime + 1s < high_resolution_clock::now())
			SetActorLocation(FVector(12450.0, 99870.0, -540.0));
		return;
	}

	if (isMoving) AddMovementInput(velocity, speed);
	SetActorLocation(netPos);

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
	if (net.objEventQue[id].empty()) return;
	auto ev = net.objEventQue[id].front();
	net.objEventQue[id].pop();

	switch (ev.type) {
	case sc_update_obj:
		MonPos = { ev.pos.x, ev.pos.y, MonPos.Z };
		velocity = { ev.velocity.x, ev.velocity.y, ev.velocity.z };
		rotation = { ev.rotation.x, ev.rotation.y, ev.rotation.z };
		netPos = MonPos;
		SetActorLocationAndRotation(MonPos, rotation, false, 0, ETeleportType::TeleportPhysics);
		isMoving = true;
		speed = 200.f;
		break;
	case sc_attack: {
		auto animInst = Cast<UCyclopsAnimInstance>(GetMesh()->GetAnimInstance());
		if (animInst != nullptr) {
			if (ev.mp == 0) animInst->Attack1();
			if (ev.mp == 1) animInst->Attack2();
			if (ev.mp == 2) animInst->Attack3();
		}
		isMoving = false;
		speed = 0.f;
		velocity = { 0,0,0 };
	}break;
	case sc_damaged: {
		auto animInst = Cast<UCyclopsAnimInstance>(GetMesh()->GetAnimInstance());
		if (animInst != nullptr) animInst->Hitreaction();
		isMoving = false;
		velocity = { 0,0,0 };
		speed = 0.f;
		hp = ev.hp;
	}break;
	case sc_dead: {
		auto animInst = Cast<UCyclopsAnimInstance>(GetMesh()->GetAnimInstance());
		if (animInst != nullptr) animInst->Hitreaction();
		isMoving = false;
		isDead = true;
		deathTime = high_resolution_clock::now();
	}break;
	case sc_set_rotation:
		speed = 0.f;
		isMoving = false;
		rotation = { ev.rotation.x,ev.rotation.y, ev.rotation.z };
		SetActorRotation(rotation);
		break;
	default:
		isMoving = false;
		break;
	}
}

void AMyMonster::BeetleUpdate() {
	if (net.objEventQue[id].empty()) return;
	auto ev = net.objEventQue[id].front();
	net.objEventQue[id].pop();

	switch (ev.type) {
	case sc_update_obj:
		MonPos = { ev.pos.x, ev.pos.y, MonPos.Z };
		velocity = { ev.velocity.x, ev.velocity.y, ev.velocity.z };
		rotation = { ev.rotation.x, ev.rotation.y, ev.rotation.z };
		netPos = MonPos;
		// SetActorLocation(MonPos);
		SetActorLocationAndRotation(MonPos, rotation, false, 0, ETeleportType::TeleportPhysics);
		isMoving = true;
		speed = 200.f;
		break;
	case sc_attack: {
		auto animInst = Cast<UBeetleAnimInstance>(GetMesh()->GetAnimInstance());
		if (animInst != nullptr) {
			if (ev.mp == 0) animInst->Attack1();
			if (ev.mp == 1) animInst->Attack2();
			if (ev.mp == 2) animInst->Attack3();
		}
		isMoving = false;
		speed = 0.f;
		velocity = { 0,0,0 };
	}break;
	case sc_damaged: {
		auto animInst = Cast<UBeetleAnimInstance>(GetMesh()->GetAnimInstance());
		if (animInst != nullptr) animInst->Hitreaction();
		isMoving = false;
		velocity = { 0,0,0 };
		speed = 0.f;
		hp = ev.hp;
	}break;
	case sc_dead: {
		auto animInst = Cast<UBeetleAnimInstance>(GetMesh()->GetAnimInstance());
		if (animInst != nullptr) animInst->Hitreaction();
		isMoving = false;
		isDead = true;
		deathTime = high_resolution_clock::now();
	}break;
	case sc_set_rotation:
		speed = 0.f;
		isMoving = false;
		rotation = { ev.rotation.x,ev.rotation.y, ev.rotation.z };
		SetActorRotation(rotation);
		break;
	default:
		isMoving = false;
		break;
	}
}

void AMyMonster::MiniGolemUpdate() {
	if (net.objEventQue[id].empty()) return;
	auto ev = net.objEventQue[id].front();
	net.objEventQue[id].pop();

	switch (ev.type) {
	case sc_update_obj:
		MonPos = { ev.pos.x, ev.pos.y, MonPos.Z };
		velocity = { ev.velocity.x, ev.velocity.y, ev.velocity.z };
		rotation = { ev.rotation.x, ev.rotation.y, ev.rotation.z };
		netPos = MonPos;
		// SetActorLocation(MonPos);
		SetActorLocationAndRotation(MonPos, rotation, false, 0, ETeleportType::TeleportPhysics);
		isMoving = true;
		speed = 200.f;
		break;
	case sc_attack: {
		auto animInst = Cast<UMiniGolemAnimInstance>(GetMesh()->GetAnimInstance());
		if (animInst != nullptr) {
			if (ev.mp == 0) animInst->Attack1();
			if (ev.mp == 1) animInst->Attack2();
			if (ev.mp == 2) animInst->Attack3();
		}
		isMoving = false;
		speed = 0.f;
		velocity = { 0,0,0 };
	}break;
	case sc_damaged: {
		auto animInst = Cast<UMiniGolemAnimInstance>(GetMesh()->GetAnimInstance());
		if (animInst != nullptr) animInst->Hitreaction();
		isMoving = false;
		velocity = { 0,0,0 };
		speed = 0.f;
		hp = ev.hp;
	}break;
	case sc_dead: {
		auto animInst = Cast<UMiniGolemAnimInstance>(GetMesh()->GetAnimInstance());
		if (animInst != nullptr) animInst->Hitreaction();
		isMoving = false;
		isDead = true;
		deathTime = high_resolution_clock::now();
	}break;
	case sc_set_rotation:
		speed = 0.f;
		isMoving = false;
		rotation = { ev.rotation.x,ev.rotation.y, ev.rotation.z };
		SetActorRotation(rotation);
		break;
	default:
		isMoving = false;
		break;
	}
}

void AMyMonster::LazardUpdate() {
	if (net.objEventQue[id].empty()) return;
	auto ev = net.objEventQue[id].front();
	net.objEventQue[id].pop();

	switch (ev.type) {
	case sc_update_obj:
		MonPos = { ev.pos.x, ev.pos.y, MonPos.Z };
		velocity = { ev.velocity.x, ev.velocity.y, ev.velocity.z };
		rotation = { ev.rotation.x, ev.rotation.y, ev.rotation.z };
		netPos = MonPos;
		// SetActorLocation(MonPos);
		SetActorLocationAndRotation(MonPos, rotation, false, 0, ETeleportType::TeleportPhysics);
		isMoving = true;
		speed = 200.f;
		break;
	case sc_attack: {
		auto animInst = Cast<ULazardAnimInstance>(GetMesh()->GetAnimInstance());
		if (animInst != nullptr) {
			if (ev.mp == 0) animInst->Attack1();
			if (ev.mp == 1) animInst->Attack2();
			if (ev.mp == 2) animInst->Attack3();
		}
		isMoving = false;
		speed = 0.f;
		velocity = { 0,0,0 };
	}break;
	case sc_damaged: {
		auto animInst = Cast<ULazardAnimInstance>(GetMesh()->GetAnimInstance());
		if (animInst != nullptr) animInst->Hitreaction();
		isMoving = false;
		velocity = { 0,0,0 };
		speed = 0.f;
		hp = ev.hp;
	}break;
	case sc_dead: {
		auto animInst = Cast<ULazardAnimInstance>(GetMesh()->GetAnimInstance());
		if (animInst != nullptr) animInst->Hitreaction();
		isMoving = false;
		isDead = true;
		deathTime = high_resolution_clock::now();
	}break;
	case sc_set_rotation:
		speed = 0.f;
		isMoving = false;
		rotation = { ev.rotation.x,ev.rotation.y, ev.rotation.z };
		SetActorRotation(rotation);
		break;
	default:
		isMoving = false;
		break;
	}
}

void AMyMonster::TrollUpdate() {
	if (net.objEventQue[id].empty()) return;
	auto ev = net.objEventQue[id].front();
	net.objEventQue[id].pop();

	switch (ev.type) {
	case sc_update_obj:
		MonPos = { ev.pos.x, ev.pos.y, MonPos.Z };
		velocity = { ev.velocity.x, ev.velocity.y, ev.velocity.z };
		rotation = { ev.rotation.x, ev.rotation.y, ev.rotation.z };
		netPos = MonPos;
		// SetActorLocation(MonPos);
		SetActorLocationAndRotation(MonPos, rotation, false, 0, ETeleportType::TeleportPhysics);
		isMoving = true;
		speed = 200.f;
		break;
	case sc_attack: {
		// auto animInst = Cast<ULazardAnimInstance>(GetMesh()->GetAnimInstance());
		// if (animInst != nullptr) {
		// 	if (ev.mp == 0) animInst->Attack1();
		// 	if (ev.mp == 1) animInst->Attack2();
		// 	if (ev.mp == 2) animInst->Attack3();
		// }
		isMoving = false;
		speed = 0.f;
		velocity = { 0,0,0 };
	}break;
	case sc_damaged: {
		// auto animInst = Cast<ULazardAnimInstance>(GetMesh()->GetAnimInstance());
		// if (animInst != nullptr) animInst->Hitreaction();
		isMoving = false;
		velocity = { 0,0,0 };
		speed = 0.f;
		hp = ev.hp;
	}break;
	case sc_dead: {
		// auto animInst = Cast<ULazardAnimInstance>(GetMesh()->GetAnimInstance());
		// if (animInst != nullptr) animInst->Hitreaction();
		isMoving = false;
		isDead = true;
		deathTime = high_resolution_clock::now();
	}break;
	case sc_set_rotation:
		speed = 0.f;
		isMoving = false;
		rotation = { ev.rotation.x,ev.rotation.y, ev.rotation.z };
		SetActorRotation(rotation);
		break;
	default:
		isMoving = false;
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