// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

extern Network net;
UMyGameInstance::UMyGameInstance()
{
	
}

void UMyGameInstance::Init()
{
	Super::Init();
	//AMyCharacter* SpawnedPlayer = GetWorld()->SpawnActor<AMyCharacter>(WhoToSpawn, SpawnLocation, SpawnRotation);
	TickDelegateHandle = FTicker::GetCoreTicker().AddTicker(FTickerDelegate::CreateUObject(this, &UMyGameInstance::Tick));
}

void UMyGameInstance::Shutdown() {
	FTicker::GetCoreTicker().RemoveTicker(TickDelegateHandle);
	Super::Shutdown();
}

bool UMyGameInstance::Tick(float DeltaTime) {
	net.my_charType = CharTypeNum;
	return true;
}
