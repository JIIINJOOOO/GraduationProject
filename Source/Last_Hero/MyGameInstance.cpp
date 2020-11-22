// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"
#include "Network.h"
int charType;
extern Network net;
UMyGameInstance::UMyGameInstance()
{

}

void UMyGameInstance::Init()
{
	Super::Init();
	//AMyCharacter* SpawnedPlayer = GetWorld()->SpawnActor<AMyCharacter>(WhoToSpawn, SpawnLocation, SpawnRotation);
	charType = CharTypeNum;
	if (net.GetStatus() != p_login) {
		CS_LOGIN p{ sizeof(CS_LOGIN), cs_login, "test", "1234", charType };
		net.SendPacket(&p);
	}
}