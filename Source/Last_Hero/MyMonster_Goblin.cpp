// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMonster_Goblin.h"
#include "MyMonsterAIController.h"
#include "MyAnimInstance.h"
#include "GoblinAnimInstance.h"
#include "Engine.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "UObject/ConstructorHelpers.h"
#include "Network.h"
#include <math.h>
extern Network net;

// Sets default values
AMyMonster_Goblin::AMyMonster_Goblin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_GOBLIN(TEXT("/Game/Game/Mesh/Monster/Monster_Goblin_Mesh/goblin_d_shareyko.goblin_d_shareyko"));
	if (SK_GOBLIN.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_GOBLIN.Object);
	}
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	// Blueprint'/Game/Game/BluePrints/MyMonster_Goblin_BP.MyMonster_Goblin_BP'
	// --------------------------------------------------------------------------------------
	// 프로젝트 켜진 상태에서 컴파일돌리면 문제가 없는데 언리얼 재시작할때는 73%에서 무한로딩걸림	
	// static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(TEXT("AnimBlueprint'/Game/Game/BluePrints/Goblin/BPA_Goblin.BPA_Goblin_C'"));
	// if (AnimBP.Succeeded()) {
	// 	GetMesh()->SetAnimInstanceClass(AnimBP.Class);
	// }
	// --------------------------------------------------------------------------------------

	// GetMesh()->SetRelativeTransform(FTransform(FVector(0.f, 0.f, -88.f)));
	//static ConstructorHelpers::FClassFinder<UAnimInstance> GOB_ANIM(TEXT("/Game/Game/BluePrints/Goblin/BPA_Goblin.BPA_Goblin_C"));

	//if (GOB_ANIM.Succeeded())
	//{
	//	GetMesh()->SetAnimInstanceClass(GOB_ANIM.Class);
	//}

	// static ConstructorHelpers::FClassFinder<UObject> GOB_AICONTROLLER(TEXT("/Game/Game/BluePrints/Goblin/Ai_Monster_Goblin.Ai_Monster_Goblin_C"));


	// if (GOB_AICONTROLLER.Succeeded())
	// {
	// 	AIControllerClass = GOB_AICONTROLLER.Class;
	// 	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	// }

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyMonster"));

	id = -1;
	// id = 10000;
	velocity = { 0.1f, 0,0 };
	speed = 0.1f;
	isMoving = false;
	isDead = false;
	hp = 100;
	type = -1;
	// isMoving = true;
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
	if (type == -1)return;
	// AddMovementInput(velocity, speed);

	/*
	x, y 각각 계산해서
	그러니까 velocity가 양수일 경우에는 상대 좌표값이 내 좌표보다 커야하고
	velocity가 음수면 내 좌표보다 작아야한다
	0 이면.. 걍 넘기자 어차피 서버에선 뷰리스트 기반으로 해서 큰 차이는 안날거야
	*/
	
	// velocity = GetVelocity();
	// float y = (velocity.X*vel.X) + (velocity.Y*vel.Y) + (velocity.Z*vel.Z);
	// float cy = cos(y);
	// UE_LOG(LogTemp, Log, TEXT("adafafasfafs  %d %lf"));;
	
	if (isDead) {
		if (deadCnt == 500) SetActorLocation(FVector(12450.0, 99870.0, -540.0));
		deadCnt++;
		return;
	}
	// float y = 180 * atan2(net.my_pos.x - MonPos.X, net.my_pos.y - MonPos.Y) / PI;
	// SetActorRotation(FRotator(0, -y, 0));
	if (isMoving) AddMovementInput(velocity, speed);
	// velocity = -velocity;
	// AddMovementInput(velocity, speed);

	// if (net.isHost && isMoving) {
	// 	MonPos = GetActorLocation();
	// 	rotation = GetActorRotation();
	// 	velocity = GetVelocity();
	// 	CS_NPC_MOVE pack{ sizeof(CS_NPC_MOVE), cs_npc_move, id };
	// 	pack.pos = { MonPos.X, MonPos.Y, MonPos.Z };
	// 	pack.roatation = { rotation.Pitch, rotation.Yaw, rotation.Roll };
	// 	pack.velocity = { velocity.X, velocity.Y, velocity.Z };
	// 	net.SendPacket(&pack);
	// }
	net.eventLock.lock();
	if (net.eventQue.empty()) {
		net.eventLock.unlock();
		return;
	}
	auto ev = net.eventQue.front();
	net.eventLock.unlock();
	
	if (ev.oid < NPC_ID_START) return;
	if (ev.oid != id) return;
	// if (net.isHost) {
	// 	net.PopEvent();
	// 	return;
	// }


	switch (ev.type) {
	case sc_update_obj: {
		// if (net.isHost) {
		// 	net.PopEvent();
		// 	break;
		// }
		AddMovementInput(velocity, speed);
		SetActorLocationAndRotation(FVector(ev.pos.x, ev.pos.y, MonPos.Z), FRotator(ev.rotation.x, ev.rotation.y, ev.rotation.z), false, 0, ETeleportType::TeleportPhysics);
		velocity = { ev.velocity.x,ev.velocity.y,ev.velocity.z };
		// SetActorRotation(FRotator(ev.rotation.x, ev.rotation.y, ev.rotation.z));
		// velocity.Normalize();
		isMoving = true;
		speed = 200.f;
		net.PopEvent();
	}break;
	case sc_dead: {
		// SetActorLocation(FVector(0, 0, 0));
		animInstance->Death();
		velocity = { 0,0,0 };
		hp = ev.hp;
		speed = 0.f;
		isDead = true;
		isMoving = false;
		net.PopEvent();
	}break;
	case sc_attack: {
		// UGoblinAnimInstance* myAnimInst = Cast<UGoblinAnimInstance>(animInstance);
		// if (myAnimInst != nullptr) myAnimInst->Slash();
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
		velocity = { 0,0,0 };
		speed = 0.f;
		isMoving = false;

		net.PopEvent();
	}break;
	case sc_move_stop: {
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
	type = OBJ_GOBLIN;
}