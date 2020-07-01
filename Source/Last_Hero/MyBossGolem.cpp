// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBossGolem.h"

// Sets default values
AMyBossGolem::AMyBossGolem()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyMonster"));
}

// Called when the game starts or when spawned
void AMyBossGolem::BeginPlay()
{
	Super::BeginPlay();
	BoneMap.Emplace("Bip001-R-Hand", 5);
	BoneMap.Emplace("Bip001-R-Forearm", 7);
	BoneMap.Emplace("Bip001-R-UpperArm", 9);
	BoneMap.Emplace("Bip001-R-Foot", 5);
	BoneMap.Emplace("Bip001-R-Calf", 7);
	BoneMap.Emplace("Bip001-R-Thigh", 9);
	BoneMap.Emplace("Bip001-L-Hand", 5);
	BoneMap.Emplace("Bip001-L-Forearm", 7);
	BoneMap.Emplace("Bip001-L-UpperArm", 9);
	BoneMap.Emplace("Bip001-L-Foot", 5);
	BoneMap.Emplace("Bip001-L-Calf", 7);
	BoneMap.Emplace("Bip001-L-Thigh", 9);
}

// Called every frame
void AMyBossGolem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void AMyBossGolem::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AMyBossGolem::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ABLOG(Warning, TEXT("Actor: %s took Damage: %f"), *GetName(), FinalDamage);
	return FinalDamage;
}