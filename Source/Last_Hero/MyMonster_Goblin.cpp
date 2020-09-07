// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMonster_Goblin.h"
#include "MyMonsterAIController.h"
#include "MyAnimInstance.h"
#include "GoblinAnimInstance.h"
#include "Engine.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "UObject/ConstructorHelpers.h"
#include "Network.h"
extern Network net;

// Sets default values
AMyMonster_Goblin::AMyMonster_Goblin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_GOBLIN(TEXT("/Game/Game/Mesh/Monster/Monster_Goblin_Mesh/goblin_d_shareyko.goblin_d_shareyko_C"));
	//if (SK_GOBLIN.Succeeded())
	//{
	//	GetMesh()->SetSkeletalMesh(SK_GOBLIN.Object);
	//}

	//GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//static ConstructorHelpers::FClassFinder<UAnimInstance> GOB_ANIM(TEXT("/Game/Game/BluePrints/Goblin/BPA_Goblin.BPA_Goblin_C"));

	//if (GOB_ANIM.Succeeded())
	//{
	//	GetMesh()->SetAnimInstanceClass(GOB_ANIM.Class);
	//}

	static ConstructorHelpers::FClassFinder<UObject> GOB_AICONTROLLER(TEXT("/Game/Game/BluePrints/Goblin/Ai_Monster_Goblin.Ai_Monster_Goblin_C"));


	if (GOB_AICONTROLLER.Succeeded())
	{
		AIControllerClass = GOB_AICONTROLLER.Class;
		AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	}

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyMonster"));

	id = -1;
	id = 10000;
	// velocity = { 1000.f,0.f,0.f };
	velocity = { 0,0,0 };
	speed = 1.f;
	isMoving = false;
	isDead = false;
	hp = 100;
}

// Called when the game starts or when spawned
void AMyMonster_Goblin::BeginPlay()
{
	Super::BeginPlay();
	SpawnDefaultController();
	animInstance = Cast<UGoblinAnimInstance>(GetMesh()->GetAnimInstance());
}

// Called every frame
void AMyMonster_Goblin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MonPos = GetActorLocation();
	
	if (isDead) {
		if (deadCnt == 500) SetActorLocation(FVector(12450.0, 99870.0, -540.0));
		deadCnt++;
		return;
	}

	AddMovementInput(velocity, speed, true);

	// static int r = 0;
	// r = (r + 1) % 360;
	// SetActorRotation(FRotator(0, r, 0));
	// return;
	if (net.isHost && isMoving) {
		MonPos = GetActorLocation();
		rotation = GetActorRotation();
		velocity = GetVelocity();
		CS_NPC_MOVE pack{ sizeof(CS_NPC_MOVE), cs_npc_move, id };
		pack.pos = { MonPos.X, MonPos.Y, MonPos.Z };
		pack.roatation = { rotation.Pitch, rotation.Yaw, rotation.Roll };
		pack.velocity = { velocity.X, velocity.Y, velocity.Z };
		net.SendPacket(&pack);
	}
	net.eventLock.lock();
	if (net.eventQue.empty()) {
		net.eventLock.unlock();
		return;
	}
	auto ev = net.eventQue.front();
	net.eventLock.unlock();
	// UE_LOG(LogTemp, Log, TEXT("ev type id %d %d"), (int)ev.type, ev.oid);
	if (ev.oid < NPC_ID_START) return;
	if (ev.oid != id) return;
	// if (net.isHost) {
	// 	net.PopEvent();
	// 	return;
	// }


	switch (ev.type) {
	case sc_update_obj:
		// if (net.isHost) {
		// 	net.PopEvent();
		// 	break;
		// }
		SetActorLocationAndRotation(FVector(ev.pos.x, ev.pos.y, ev.pos.z), FRotator(ev.rotation.x, ev.rotation.y, ev.rotation.z), false, 0, ETeleportType::None);
		velocity = { ev.velocity.x,ev.velocity.y,ev.velocity.z };
		// SetActorRotation(FRotator(0, ev.rotation.y, 0));
		// velocity.Normalize();
		isMoving = true;
		speed = 200.f;
		net.PopEvent();
		break;
	case sc_dead: {
		// SetActorLocation(FVector(0, 0, 0));
		animInstance->Death();
		//velocity = { 0,0,0 };
		hp = ev.hp;
		speed = 0.f;
		isDead = true;
		isMoving = false;
		net.PopEvent();
	}break;
	case sc_attack: {
		UGoblinAnimInstance* myAnimInst = Cast<UGoblinAnimInstance>(animInstance);
		if (myAnimInst != nullptr) myAnimInst->Slash();
		//velocity = { 0,0,0 };
		speed = 0.f;
		isMoving = false;

		net.PopEvent();
	}break;
	case sc_damaged: {
		UGoblinAnimInstance* myAnimInst = Cast<UGoblinAnimInstance>(animInstance);
		if (myAnimInst != nullptr) myAnimInst->HitReaction();
		hp = ev.hp;
		speed = 0.f;
		isMoving = false;

		net.PopEvent();
	}break;
	case sc_move_stop: {
		isMoving = false;
		velocity = { 0,0,0 };
		speed = 0.f;
		isMoving = false;

		net.PopEvent();
	}break;
	case sc_set_npc_target: {
		//isMoving = true;
		//velocity = { ev.pos.x, ev.pos.y ,ev.pos.z };
		//UE_LOG(LogTemp, Log, TEXT("mon1set %d %d %d"), velocity.X, velocity.Y, velocity.Z);
		//(LogTemp, Log, TEXT("mon1set2 %d %d %d"), ev.pos.x, ev.pos.y, ev.pos.z);
		// speed = 200.f;

		net.PopEvent();
	}break;
	case sc_block:
		isMoving = false;

		net.PopEvent();
		break;
	default:
		net.PopEvent();
		break;
	}
}

// Called to bind functionality to input
void AMyMonster_Goblin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AMyMonster_Goblin::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ABLOG(Warning, TEXT("Actor: %s took Damage: %f"), *GetName(), FinalDamage);
	return FinalDamage;
}


FVector AMyMonster_Goblin::GetMonsterPos()
{	
	return MonPos;
}

void AMyMonster_Goblin::SetID(const int& id) {
	this->id = id;
}