// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMonster.h"
#include "Network.h"
extern Network net;

// Sets default values
AMyMonster::AMyMonster()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	id = -1;
}

// Called when the game starts or when spawned
void AMyMonster::BeginPlay()
{
	Super::BeginPlay();
	MonPos = GetActorLocation();
}

// Called every frame
void AMyMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	MonPos = GetActorLocation();
	return;
	// net.eventLock.lock();
	// if (net.eventQue.empty()) {
	// 	net.eventLock.unlock();
	// 	return;
	// }
	// auto ev = net.eventQue.front();
	// net.eventLock.unlock();
	// if (ev.oid < NPC_ID_START) return;
	// if (ev.oid != id) return;
	// 
	// switch (ev.type) {
	// case sc_update_obj:
	// 	SetActorLocationAndRotation(FVector(ev.pos.x, ev.pos.y, ev.pos.z), FRotator// (ev.rotation.x, ev.rotation.y, ev.rotation.z), false, 0, ETeleportType::None);
	// 	net.PopEvent();
	// 	break;
	// case sc_dead:
	// 	SetActorLocation(FVector(0, 0, 0));
	// 	net.PopEvent();
	// 	break;
	// case sc_attack:
	// 	net.PopEvent();
	// 	break;
	// case sc_damaged:
	// 	net.PopEvent();
	// 	break;
	// case sc_level_up:
	// 	net.PopEvent();
	// 	break;
	// case sc_block:
	// 	net.PopEvent();
	// 	break;
	// }
}

// Called to bind functionality to input
void AMyMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMyMonster::SetID(const int& id) {
	this->id = id;
}
