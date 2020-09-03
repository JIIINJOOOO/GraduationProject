// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyCharacter.h"
#include "MyMonster.h"
#include "MyBossGolem.h"
#include "MyMonster_Goblin.h"
#include "Monster_Goblin.h"
#include "MyPlayerController.h"

#define MAX_PLAYER 10

// Global Values
Network net;	// 다른 소스파일에서 extern으로 가져다 씀

AMyGameModeBase::AMyGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = AMyCharacter::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
	MonToSpawn = AMyMonster_Goblin::StaticClass();
	BossToSpawn = AMyBossGolem::StaticClass();
	/*static ConstructorHelpers::FObjectFinder<UBlueprint> GOBLIN(TEXT("Blueprint'/Game/Game/BluePrints/Goblin/Monster_BP_2.Monster_BP_2'"));
	if (GOBLIN.Object) {
		MonsterBP = GOBLIN.Object;
	}*/
	static ConstructorHelpers::FObjectFinder<UBlueprint> GOBLIN_AI(TEXT("Blueprint'/Game/Game/BluePrints/Goblin/Ai_Monster_Goblin.Ai_Monster_Goblin'"));
	if (GOBLIN_AI.Object) {
		MonsterAIBP = GOBLIN_AI.Object;
	}
	//static ConstructorHelpers::FClassFinder<ACharacter> BP_CHARACTER_C(TEXT("/Game/Game/BluePrints/ThirdPersonCharacter.ThirdPersonCharacter_C"));

	/*if (BP_CHARACTER_C.Succeeded())
	{
		DefaultPawnClass = BP_CHARACTER_C.Class;
	}*/
}

void AMyGameModeBase::PostLogin(APlayerController * NewPlayer)
{
	// 플레이어 스폰 위치 트랜스폼
	FTransform PlayerSpawnTrans(FRotator::ZeroRotator, FVector(14410.0f, 77670.0f, -450.0f), FVector(1.0f, 1.0f, 1.0f));
	// 플레이어 스폰 코드
	RestartPlayerAtTransform(NewPlayer, PlayerSpawnTrans);
	Super::PostLogin(NewPlayer);
	// 플레이어 얻어오기
	// auto MyCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(),0);
}

void AMyGameModeBase::BeginPlay()
{
	SpawnMonster();
	// SpawnBoss(5, 0, 0, 0);
}

void AMyGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ProcessEvent();
}


void AMyGameModeBase::SpawnMonster()
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoFail = true;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = NULL;
	SpawnInfo.bDeferConstruction = false;
	// 몬스터 스폰 위치(일단 한마리 위치만 테스트용으로)
	FVector MonSpawnLocation = { 14410.f, 88350.921875f, -447.422394f };
	// 몬스터 스폰 코드
	AMyMonster_Goblin* SpawnMonster = GetWorld()->SpawnActor<AMyMonster_Goblin>(MonToSpawn, MonSpawnLocation, FRotator::ZeroRotator, SpawnInfo); // 이렇게 하면 블프에서 구현해놓은 AI 구동이 안된다
	/*AActor* SpawnMonster = GetWorld()->SpawnActor(MonsterBP->GeneratedClass);
	SpawnMonster->SetActorLocation(MonSpawnLocation);*/

}

void AMyGameModeBase::SpawnMonster(int oid, float x, float y, float z) {
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoFail = true;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = NULL;
	SpawnInfo.bDeferConstruction = false;
	// 몬스터 스폰 위치(일단 한마리 위치만 테스트용으로)
	FVector MonSpawnLocation = { x,y,z };
	// 몬스터 스폰 코드
	AMyMonster_Goblin* SpawnMonster = GetWorld()->SpawnActor<AMyMonster_Goblin>(MonToSpawn, MonSpawnLocation, FRotator::ZeroRotator, SpawnInfo); // 이렇게 하면 블프에서 구현해놓은 AI 구동이 안된다
	SpawnMonster->SetID(oid);
	// monsters.insert(oid, SpawnMonster);
	/*AActor* SpawnMonster = GetWorld()->SpawnActor(MonsterBP->GeneratedClass);
	SpawnMonster->SetActorLocation(MonSpawnLocation);*/
}

void AMyGameModeBase::SpawnBoss(int oid, float x, float y, float z) {
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoFail = true;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = NULL;
	SpawnInfo.bDeferConstruction = false;
	// 몬스터 스폰 위치(일단 한마리 위치만 테스트용으로)
	FVector SpawnLocation = { x,y,z };
	// 몬스터 스폰 코드
	auto bossGolem = GetWorld()->SpawnActor<AMyBossGolem>(BossToSpawn, SpawnLocation, FRotator::ZeroRotator, SpawnInfo);
	bossGolem->SetID(oid);
}

void AMyGameModeBase::SpawnPlayer(int oid, float x, float y, float z) {
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoFail = true;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = NULL;
	SpawnInfo.bDeferConstruction = false;
	FVector SpawnLocation = { x,y,z };
	auto MyChar = GetWorld()->SpawnActor<AMyCharacter>(DefaultPawnClass, SpawnLocation, FRotator::ZeroRotator, SpawnInfo);
	MyChar->SetID(oid);
}

void AMyGameModeBase::ProcessEvent() {
	net.eventLock.lock();
	if (net.eventQue.empty()) {
		net.eventLock.unlock();
		return;
	}
	auto ev = net.eventQue.front();
	net.eventLock.unlock();

	switch (ev.type) {
	case sc_enter_obj: {
		if (ev.oid < MAX_PLAYER) {
			SpawnPlayer(ev.oid, ev.pos.x, ev.pos.y, ev.pos.z);
		}
		else if (ev.oid == 20000) {
			// UE_LOG(LogTemp, Log, TEXT("Boss Spawn in GMB"));
			// SpawnBoss(ev.oid, ev.pos.x, ev.pos.y, ev.pos.z);
		}
		else {
			// SpawnMonster(ev.oid, ev.pos.x, ev.pos.y, ev.pos.z);
			// SpawnMonster();
		}
		net.PopEvent();
	}break;

	default:
		break;
	}
}