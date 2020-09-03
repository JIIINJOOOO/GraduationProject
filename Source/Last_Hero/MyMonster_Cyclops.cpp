// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMonster_Cyclops.h"

// Sets default values
AMyMonster_Cyclops::AMyMonster_Cyclops()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyMonster_Cyclops::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyMonster_Cyclops::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyMonster_Cyclops::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

