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
	netPos = GetActorLocation();
	SetActorRotation(FRotator(0, 180, 0));
}

// Called every frame
void AMyMonster_Goblin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MonPos = GetActorLocation();
	netPos.Z = MonPos.Z;

	if (type == -1) return;
	if (id == -1) return;

	if (isDead) {
		if (deathTime + 5s < high_resolution_clock::now())
			SetActorLocation(FVector(12450.0, 99870.0, -540.0));
		return;
	}

	if (isMoving) AddMovementInput(velocity, speed);
	SetActorLocation(netPos);

	if (net.objEventQue[id].empty()) return;
	auto ev = net.objEventQue[id].front();
	net.objEventQue[id].pop();
	switch (ev.type) {
	case sc_update_obj:
		MonPos = { ev.pos.x, ev.pos.y, MonPos.Z };
		velocity = { ev.velocity.x,ev.velocity.y,ev.velocity.z };
		rotation = { ev.rotation.x, ev.rotation.y, ev.rotation.z };
		netPos = MonPos;
		// SetActorLocationAndRotation(MonPos, rotation, false, 0, ETeleportType::None);
		SetActorLocation(MonPos);
		// SetActorRotation(FRotator(0, ev.rotation.y, 0));
		// velocity.Normalize();
		isMoving = true;
		speed = 200.f;
		break;
	case sc_dead: {
		animInstance->Death();
		velocity = { 0,0,0 };
		hp = ev.hp;
		speed = 0.f;
		isDead = true;
		deathTime = high_resolution_clock::now();
		isMoving = false;
	}break;
	case sc_attack: {
		if (ev.mp == 0)	animInstance->Slash();
		if (ev.mp == 1) animInstance->AttackBackhand();
		if (ev.mp == 2) animInstance->AttackDownward();
		velocity = { 0,0,0 };
		speed = 0.f;
		isMoving = false;
	}break;
	case sc_damaged: {
		animInstance->HitReaction();
		hp = ev.hp;
		speed = 0.f;
		isMoving = false;
	}break;
	case sc_move_stop: {
		hp = ev.hp;
		velocity = { 0,0,0 };
		speed = 0.f;
		isMoving = false;
	}break;
	case sc_block:
		isMoving = false;
		break;
	case sc_set_pos:
		velocity = { 0,0,0 };
		speed = 0.f;
		isMoving = false;
		MonPos = { ev.pos.x, ev.pos.y, ev.pos.z };
		netPos = MonPos;
		SetActorLocation(MonPos);
		break;
	case sc_set_rotation:
		speed = 0.f;
		isMoving = false;
		rotation = { ev.rotation.x,ev.rotation.y, ev.rotation.z };
		SetActorRotation(rotation);
		break;
	default:
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