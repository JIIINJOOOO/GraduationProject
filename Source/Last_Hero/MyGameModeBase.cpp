// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyCharacter.h"
#include "MyMonster_Goblin.h"
#include "MyBossGolem.h"
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
	CycToSpawn = AMyMonster::StaticClass();
	/*static ConstructorHelpers::FObjectFinder<UBlueprint> GOBLIN(TEXT("Blueprint'/Game/Game/BluePrints/Goblin/Monster_BP_2.Monster_BP_2'"));
	if (GOBLIN.Object) {
		MonsterBP = GOBLIN.Object;
	}*/

	static ConstructorHelpers::FClassFinder<UObject> GOBLIN_AI(TEXT("/Game/Game/BluePrints/Goblin/Ai_Monster_Goblin.Ai_Monster_Goblin_C"));
	if (GOBLIN_AI.Succeeded()) {
		MonsterAIBP = GOBLIN_AI.Class;
	}


	// GetMesh()->SetAnimInstanceClass(AnimBP.Class);
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
	// SpawnGoblin(0, 8195.728516, 75672.898438, -443.441223);
	// SpawnCyclops(0, 15429.8f, 78471.57f, -385.f);
	// SpawnBeetle(0, 15429.8f, 78471.57f, -445.6f);
	// SpawnMiniGolem(0, 15429.8f, 78471.57f, -445.6f);
	// SpawnLazard(0, 15429.8f, 78471.57f, -445.6f);

	// UE_LOG(LogTemp, Log, TEXT("GMB BeginPlay"));
	if (net.GetStatus() != p_login) {
		CS_LOGIN p{ sizeof(CS_LOGIN), cs_login, "test", "1234" };
		net.SendPacket(&p);
	}
}

void AMyGameModeBase::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	ProcessEvent2();
}


void AMyGameModeBase::SpawnMonster()
{
	FName path = TEXT("Blueprint'/Game/Game/BluePrints/Goblin/Monster_BP_3.Monster_BP_3'");
	auto obj = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), NULL, *path.ToString()));
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoFail = true;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = NULL;
	SpawnInfo.bDeferConstruction = false;
	// 몬스터 스폰 위치(일단 한마리 위치만 테스트용으로)
	FVector MonSpawnLocation = { 15429.8f,78471.57f,-445.6f };
	// 15429.818359   78471.570312   -445.642273
	// 몬스터 스폰 코드 -> 컨트롤러를 가져와야 할 듯

	auto SpawnMonster = GetWorld()->SpawnActor<AMyMonster_Goblin>(obj->GeneratedClass, MonSpawnLocation, FRotator::ZeroRotator, SpawnInfo); // 이렇게 하면 블프에서 구현해놓은 AI 구동이 안된다
	/*AActor* SpawnMonster = GetWorld()->SpawnActor(MonsterBP->GeneratedClass);
	SpawnMonster->SetActorLocation(MonSpawnLocation);*/
	SpawnMonster->SetID(-1);
}

void AMyGameModeBase::SpawnGoblin(const int& oid, float x, float y, float z) {
	FName path = TEXT("Blueprint'/Game/Game/BluePrints/Goblin/Monster_BP_3.Monster_BP_3'");
	auto obj = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), NULL, *path.ToString()));
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoFail = true;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = NULL;
	SpawnInfo.bDeferConstruction = false;
	FVector MonSpawnLocation = { x, y, z };
	auto SpawnMonster = GetWorld()->SpawnActor<AMyMonster_Goblin>(obj->GeneratedClass, MonSpawnLocation, FRotator::ZeroRotator, SpawnInfo);
	SpawnMonster->SetID(oid);
}

void AMyGameModeBase::SpawnCyclops(const int& oid, float x, float y, float z) {
	FName path = TEXT("Blueprint'/Game/Game/BluePrints/Cyclops/BP_Cyclops.BP_Cyclops'");
	auto obj = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), NULL, *path.ToString()));
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoFail = true;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = NULL;
	SpawnInfo.bDeferConstruction = false;
	// 몬스터 스폰 위치(일단 한마리 위치만 테스트용으로)
	FVector MonSpawnLocation = { x, y, z };
	auto SpawnMonster = GetWorld()->SpawnActor<AMyMonster>(obj->GeneratedClass, MonSpawnLocation, FRotator::ZeroRotator, SpawnInfo);
	SpawnMonster->SetType(OBJ_CYCLOPS);
	SpawnMonster->SetID(oid);
}

void AMyGameModeBase::SpawnBeetle(const int& oid, float x, float y, float z) {
	FName path = TEXT("Blueprint'/Game/Game/BluePrints/Beetle_Warrior/BP_Beetle_Warrior.BP_Beetle_Warrior'");
	auto obj = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), NULL, *path.ToString()));
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoFail = true;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = NULL;
	SpawnInfo.bDeferConstruction = false;
	// 몬스터 스폰 위치(일단 한마리 위치만 테스트용으로)
	FVector MonSpawnLocation = { x, y, z };
	auto SpawnMonster = GetWorld()->SpawnActor<AMyMonster>(obj->GeneratedClass, MonSpawnLocation, FRotator::ZeroRotator, SpawnInfo);
	SpawnMonster->SetType(OBJ_BEETLE);
	SpawnMonster->SetID(oid);
}

void AMyGameModeBase::SpawnMiniGolem(const int& oid, float x, float y, float z) {
	FName path = TEXT("Blueprint'/Game/Game/BluePrints/mini_golem/Mini_golem.Mini_golem'");
	auto obj = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), NULL, *path.ToString()));
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoFail = true;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = NULL;
	SpawnInfo.bDeferConstruction = false;
	// 몬스터 스폰 위치(일단 한마리 위치만 테스트용으로)
	FVector MonSpawnLocation = { x, y, z };
	auto SpawnMonster = GetWorld()->SpawnActor<AMyMonster>(obj->GeneratedClass, MonSpawnLocation, FRotator::ZeroRotator, SpawnInfo);
	SpawnMonster->SetType(OBJ_MINI_GOLEM);
	SpawnMonster->SetID(oid);
}

void AMyGameModeBase::SpawnLazard(const int& oid, float x, float y, float z) {
	FName path = TEXT("Blueprint'/Game/Game/BluePrints/Lazardman/BP_Lazardman.BP_Lazardman'");
	auto obj = Cast<UBlueprint>(StaticLoadObject(UBlueprint::StaticClass(), NULL, *path.ToString()));
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoFail = true;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = NULL;
	SpawnInfo.bDeferConstruction = false;
	// 몬스터 스폰 위치(일단 한마리 위치만 테스트용으로)
	FVector MonSpawnLocation = { x, y, z };
	auto SpawnMonster = GetWorld()->SpawnActor<AMyMonster>(obj->GeneratedClass, MonSpawnLocation, FRotator::ZeroRotator, SpawnInfo);
	SpawnMonster->SetType(OBJ_LAZARD);
	SpawnMonster->SetID(oid);
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

void AMyGameModeBase::ProcessEvent2() {
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
		// else if (ev.oid == 20000) {
		// 	// SpawnBoss(ev.oid, ev.pos.x, ev.pos.y, ev.pos.z);
		// }
		else {	// Normal Monster
			if (ev.o_type == OBJ_GOBLIN)
				SpawnGoblin(ev.oid, ev.pos.x, ev.pos.y, ev.pos.z);
			else if (ev.o_type == OBJ_CYCLOPS)
				SpawnCyclops(ev.oid, ev.pos.x, ev.pos.y, ev.pos.z);
			else if (ev.o_type == OBJ_BEETLE)
				SpawnBeetle(ev.oid, ev.pos.x, ev.pos.y, ev.pos.z);
			else if (ev.o_type == OBJ_MINI_GOLEM)
				SpawnMiniGolem(ev.oid, ev.pos.x, ev.pos.y, ev.pos.z);
			else if (ev.o_type == OBJ_LAZARD)
				SpawnLazard(ev.oid, ev.pos.x, ev.pos.y, ev.pos.z);
		}
		net.PopEvent();
	}break;

	default:
		break;
	}
}