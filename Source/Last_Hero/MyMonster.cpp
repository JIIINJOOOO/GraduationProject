// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMonster.h"

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

}

// Called to bind functionality to input
void AMyMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

