// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMonster.h"
#include "Network.h"
#include "CyclopsAnimInstance.h"
#include "BeetleAnimInstance.h"
#include "MiniGolemAnimInstance.h"

extern Network net;

// Sets default values
AMyMonster::AMyMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	static ConstructorHelpers::FClassFinder<UObject> MON_AICONTROLLER(TEXT("/Game/Game/BluePrints/mini_golem/Ai_Monster_minigolem.Ai_Monster_minigolem_C"));


	if (MON_AICONTROLLER.Succeeded())
	{
		AIControllerClass = MON_AICONTROLLER.Class;
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	}

	PrimaryActorTick.bCanEverTick = true;
	speed = 200.f;
	velocity = { 0,0,0 };
	isDead = false;
	id = 0;
}

// Called when the game starts or when spawned
void AMyMonster::BeginPlay()
{
	Super::BeginPlay();
	MonPos = GetActorLocation();
	
	
	id = 10000 + net.mon_num;
	if (net.isHost) {
		CS_SPAWN_NPC pack{ sizeof(pack), cs_spawn_npc, id };
		pack.pos = { MonPos.X, MonPos.Y, MonPos.Z };
		net.SendPacket(&pack);
	}
}

// Called every frame
void AMyMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MonPos = GetActorLocation();

	AddMovementInput(velocity, speed, true);

	if (net.isHost == true) {
		MonPos = GetActorLocation();
		rotation = GetActorRotation();
		velocity = GetVelocity();
		CS_NPC_MOVE pack{ sizeof(CS_NPC_MOVE), cs_npc_move, id };
		pack.pos = { MonPos.X, MonPos.Y, MonPos.Z };
		pack.roatation = { rotation.Pitch, rotation.Yaw, rotation.Roll };
		pack.velocity = { velocity.X, velocity.Y, velocity.Z };
		net.SendPacket(&pack);
	}

	// else {	// is not host client
	net.eventLock.lock();
	if (net.eventQue.empty()) {
		net.eventLock.unlock();
		return;
	}
	auto ev = net.eventQue.front();
	net.eventLock.unlock();
	if (ev.oid < NPC_ID_START) return;
	if (ev.oid != id) return;
	
	if (isDead) {
		SetActorLocation(FVector(12450.0, 99870.0, -540.0));
		return;
	}

	switch (ev.type) {
	case sc_update_obj:
		if (net.isHost) {
			net.PopEvent();
			break;
		}
		MonPos = { ev.pos.x, ev.pos.y, ev.pos.z };
		rotation = { ev.rotation.x, ev.rotation.y, ev.rotation.z };
		velocity = { ev.velocity.x, ev.velocity.y, ev.velocity.z };
		SetActorLocationAndRotation(MonPos, rotation, false, 0, ETeleportType::None);
		speed = 200.f;
		net.PopEvent();
		break;
	case sc_attack: {
		/*if (IsCyclops()) {
			auto animInst = Cast<UCyclopsAnimInstance>(GetMesh()->GetAnimInstance());
			if (animInst != nullptr) animInst->Attack1();
		}
		else if (IsBeetle()) {
			auto animInst = Cast<UBeetleAnimInstance>(GetMesh()->GetAnimInstance());
			if (animInst != nullptr) animInst->Attack1();
		}
		else if (IsMiniGolem()) {
			auto animInst = Cast<UMiniGolemAnimInstance>(GetMesh()->GetAnimInstance());
			if (animInst != nullptr) animInst->Attack1();
		}*/
		speed = 0.f;
		net.PopEvent();
	}break;
	case sc_damaged: {
		/*if (IsCyclops()) {
			auto animInst = Cast<UCyclopsAnimInstance>(GetMesh()->GetAnimInstance());
			if (animInst != nullptr) animInst->Hitreaction();
		}
		else if (IsBeetle()) {
			auto animInst = Cast<UBeetleAnimInstance>(GetMesh()->GetAnimInstance());
			if (animInst != nullptr) animInst->Hitreaction();
		}
		else if (IsMiniGolem()) {
			auto animInst = Cast<UMiniGolemAnimInstance>(GetMesh()->GetAnimInstance());
			if (animInst != nullptr) animInst->Hitreaction();
		}*/
		hp = ev.hp;
		speed = 0.f;
		net.PopEvent();
	}break;
	case sc_block:
		speed = 0.f;
		net.PopEvent();
		break;
	case sc_dead: {
		isDead = true;
		SetActorLocation(FVector(12450.0, 99870.0, -540.0));
		speed = 0.f;
		net.PopEvent();
	}break;
	default:
		speed = 0.f;
		net.PopEvent();
		break;
	}
}

// Called to bind functionality to input
void AMyMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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