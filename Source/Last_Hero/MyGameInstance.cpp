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
	if (net.objEventQue[GI_ID].empty()) return true;
	auto ev = net.objEventQue[GI_ID].front();
	net.objEventQue[GI_ID].pop();

	if (ev.type != sc_enter_obj) return true;
	if (ev.oid >= MAX_USERS) return true;
	CharTypeNum = ev.o_type;
	// net.my_charType = CharTypeNum;
	atomic_thread_fence(memory_order_seq_cst);
	net.objEventQue[GMB_ID].push(ev);
	return true;
}
