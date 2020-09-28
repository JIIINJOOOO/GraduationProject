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
	velocity = { 0,0,0 };
	speed = 0.1f;
	isMoving = false;
	isDead = false;
	hp = 100;
	type = -1;
	speed = 0.f;
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
	
	if (type == -1) return;

	if (isDead) {
		if (deadCnt == 300) SetActorLocation(FVector(12450.0, 99870.0, -540.0));
		deadCnt++;
		return;
	}

	if (isMoving) AddMovementInput(velocity, speed);

	if (net.isHost) {
		CS_NPC_MOVE pack{ sizeof(CS_NPC_MOVE), cs_npc_move, id };
		pack.pos = { MonPos.X,MonPos.Y,MonPos.Z };
		net.SendPacket(&pack);
	}

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
		MonPos = { ev.pos.x, ev.pos.y, ev.pos.z };
		velocity = { ev.velocity.x,ev.velocity.y,ev.velocity.z };
		rotation = { ev.rotation.x, ev.rotation.y, ev.rotation.z };
		// SetActorLocationAndRotation(MonPos, rotation, false, 0, ETeleportType::None);
		SetActorLocation(MonPos);
		// SetActorRotation(FRotator(0, ev.rotation.y, 0));
		// velocity.Normalize();
		isMoving = true;
		speed = 0.1f;
		speed = 200.f;
		net.PopEvent();
		break;
	case sc_dead: {
		animInstance->Death();
		velocity = { 0,0,0 };
		hp = ev.hp;
		speed = 0.f;
		isDead = true;
		isMoving = false;
		net.PopEvent();
	}break;
	case sc_attack: {
		animInstance->Slash();
		velocity = { 0,0,0 };
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
		hp = ev.hp;
		velocity = { 0,0,0 };
		speed = 0.f;
		isMoving = false;

		net.PopEvent();
	}break;
	case sc_set_npc_target: {

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
	type = OBJ_GOBLIN;
}