// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMonster_Goblin.h"
#include "Network.h"
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

	// GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	// static ConstructorHelpers::FClassFinder<UAnimInstance> GOB_ANIM(TEXT("/Game/Game/BluePrints/Goblin/BPA_Goblin.BPA_Goblin"));
	
	// if (GOB_ANIM.Succeeded())
	// {
	// 	GetMesh()->SetAnimInstanceClass(GOB_ANIM.Class);
	// }

	// static ConstructorHelpers::FClassFinder<UObject> GOB_AICONTROLLER(TEXT("/Game/Game/BluePrints/Goblin/Ai_Monster_Goblin.Ai_Monster_Goblin_C"));


	// if (GOB_AICONTROLLER.Succeeded())
	// {
	// 	AIControllerClass = GOB_AICONTROLLER.Class;
	// 	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	// }


	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyMonster"));

	
	id = 10000;
	velocity = { 0,0,0 };
	speed = 0.f;
}

// Called when the game starts or when spawned
void AMyMonster_Goblin::BeginPlay()
{
	Super::BeginPlay();
	SpawnDefaultController();
	// animInstance = Cast<UGoblinAnimInstance>(GetMesh()->GetAnimInstance());

}

void AMyMonster_Goblin::PostInitializeComponents() {
	Super::PostInitializeComponents();
	animInstance = Cast<UGoblinAnimInstance>(GetMesh()->GetAnimInstance());
	// animInstance = Cast<UGoblinAnimInstance>(UGoblinAnimInstance::StaticClass);
	
}

// Called every frame
void AMyMonster_Goblin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector v = { 11330.0, 77830.0,0.f };
	v.X -= GetActorLocation().X;
	v.Y -= GetActorLocation().Y;

	AddMovementInput(velocity, speed);
	// static int r = 0;
	// r = (r + 1) % 360;
	// SetActorRotation(FRotator(0, r, 0));
	static int a = 0;
	if (a == 200) {
		UGoblinAnimInstance* myAnimInst = Cast<UGoblinAnimInstance>(GetMesh()->GetAnimInstance());
		if (myAnimInst != nullptr) myAnimInst->Slash();
		a = 0;
	}
	a++;
	MonPos = GetActorLocation();
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
		SetActorLocationAndRotation(FVector(ev.pos.x, ev.pos.y, ev.pos.z), FRotator(ev.rotation.x, ev.rotation.y, ev.rotation.z), false, 0, ETeleportType::None);
		velocity = { ev.rotation.x,ev.rotation.y,ev.rotation.z };
		speed = 200.f;
		net.PopEvent();
		break;
	case sc_dead: {
		SetActorLocation(FVector(0, 0, 0));
		velocity = { 0,0,0 };
		speed = 0.f;
		net.PopEvent();
	}break;
	case sc_attack: {
		UGoblinAnimInstance* myAnimInst = Cast<UGoblinAnimInstance>(animInstance);
		if (myAnimInst != nullptr) myAnimInst->Slash();
		velocity = { 0,0,0 };
		speed = 0.f;
		net.PopEvent();
	}break;
	case sc_damaged: {
		UGoblinAnimInstance* myAnimInst = Cast<UGoblinAnimInstance>(animInstance);
		if (myAnimInst != nullptr) myAnimInst->HitReaction();
		velocity = { 0,0,0 };
		speed = 0.f;
		net.PopEvent();
	}break;
	case sc_block:
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