// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMonster_Goblin.h"

// Sets default values
AMyMonster_Goblin::AMyMonster_Goblin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyMonster"));
	
}

// Called when the game starts or when spawned
void AMyMonster_Goblin::BeginPlay()
{
	Super::BeginPlay();
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_GOBLIN(TEXT("/Game/Game/Mesh/Monster/Monster_Goblin_Mesh/goblin_d_shareyko.goblin_d_shareyko"));
	if (SK_GOBLIN.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_GOBLIN.Object);
	}
	static ConstructorHelpers::FClassFinder<UAnimInstance> GOB_ANIM(TEXT("/Game/Game/BluePrints/Goblin/BPA_Goblin.BPA_Goblin"));

	if (GOB_ANIM.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(GOB_ANIM.Class);
	}
}

// Called every frame
void AMyMonster_Goblin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MonPos = GetActorLocation();
	
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

