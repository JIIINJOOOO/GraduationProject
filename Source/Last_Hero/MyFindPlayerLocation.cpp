// Fill out your copyright notice in the Description page of Project Settings.


#include "MyFindPlayerLocation.h"
#include "Engine/Engine.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
AMyFindPlayerLocation::AMyFindPlayerLocation()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyFindPlayerLocation::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMyFindPlayerLocation::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector MyCharacter = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	// screen log player location
	//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("Player Location: %s"), *MyCharacter.ToString()));
}

