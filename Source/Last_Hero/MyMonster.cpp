// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMonster.h"
#include "Network.h"
extern Network net;
// Sets default values
AMyMonster::AMyMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	/*static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_GOBLIN(TEXT("/Game/Game/Mesh/Monster/Monster_Goblin_Mesh/goblin_d_shareyko.goblin_d_shareyko"));
	if (SK_GOBLIN.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_GOBLIN.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> GOB_ANIM(TEXT("/Game/Game/BluePrints/Animation/Monster/Goblin_Monster_AnimBp.Goblin_Monster_AnimBp"));

	if (GOB_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(GOB_ANIM.Class);
	}*/
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyMonster"));
	
}

// Called when the game starts or when spawned
void AMyMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// UE_LOG(LogTemp, Log, TEXT("Monster Tick"));
	// if (objectID == net.gmb.oid) {;
	// 	SetActorLocation(FVector(net.gmb.pos.x, net.gmb.pos.y, net.gmb.pos.z));
	// 	net.gmb.oid = -1;
	// }
	// GMB를 거쳐서 처리한다.
	net.eventLock.lock();
	if (net.eventQue.empty()) {
		net.eventLock.unlock();
		return;
	}
	auto ev = net.eventQue.front();
	net.eventLock.unlock();
	if (ev.oid < NPC_ID_START) return;
	if (ev.oid == objectID) {
		// 1. 좌표 이동
		UE_LOG(LogTemp, Log, TEXT("Call Move"));
		SetActorLocation(FVector(ev.pos.x, ev.pos.y, ev.pos.z));
		// 2. 애니메이션 실행
		// 애니메이션 실행 관련 코드 받아서 진행
		// 3. 대미지 처리
		hp = ev.hp;
		net.eventLock.lock();
		net.eventQue.pop();
		net.eventLock.unlock();
	}

}

// Called to bind functionality to input
void AMyMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AMyMonster::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ABLOG(Warning, TEXT("Actor: %s took Damage: %f"), *GetName(), FinalDamage);
	return FinalDamage;
}


FVector AMyMonster::GetMonsterPos()
{	
	return MonPos;
}

void AMyMonster::SetObjectID(int oid) {
	objectID = oid;
}