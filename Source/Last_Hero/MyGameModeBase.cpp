// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyCharacter.h"
#include "MyMonster.h"
#include "MyBossGolem.h"
#include "MyPlayerController.h"
#include <thread>
#include <map>
extern HANDLE gmbEvent;
Network net;
int num = 0;

AMyGameModeBase::AMyGameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = AMyCharacter::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
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
	// gmbEvent = CreateEvent(NULL, FALSE, FALSE, NULL)

// 	std::thread th{ GameThread };
	// HANDLE gmbthread = CreateThread(NULL, 0, GameThread, this, 0, NULL);
	
	// 로그인창 먼저 들어가기 귀찮ㅇ,ㅁ
	CS_LOGIN p;
	p.size = sizeof(CS_LOGIN);
	p.type = login_packet;
	strcpy_s(p.id, "test");
	strcpy_s(p.password, "1234");
	// net.SendPacket(&p);
	// recv(net.m_sock, net.recvBuf, BUFSIZE, 0);
	// net.ProcessPacket(net.recvBuf);
}

void AMyGameModeBase::PostLogin(APlayerController * NewPlayer)
{
	// APlayerController * NewPlayer;
	// 플레이어 스폰 위치 트랜스폼
	FTransform PlayerSpawnTrans(FRotator::ZeroRotator, FVector(14410.0f, 77670.0f, -450.0f), FVector(1.0f, 1.0f, 1.0f));
	// 플레이어 스폰 코드
	RestartPlayerAtTransform(NewPlayer, PlayerSpawnTrans);
	Super::PostLogin(NewPlayer);
}

void AMyGameModeBase::BeginPlay()
{
	//SpawnMonster();
}


void AMyGameModeBase::SpawnMonster()
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoFail = true;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = NULL;
	SpawnInfo.bDeferConstruction = false;
	// 몬스터 스폰 위치(일단 한마리 위치만 테스트용으로)
	FVector MonSpawnLocation = { 19266.0f,78928.0f,-440.0f };
	// 몬스터 스폰 코드
	AMyMonster* SpawnMonster = GetWorld()->SpawnActor<AMyMonster>(MonToSpawn, MonSpawnLocation, FRotator::ZeroRotator, SpawnInfo); // 이렇게 하면 블프에서 구현해놓은 AI 구동이 안된다
	/*AActor* SpawnMonster = GetWorld()->SpawnActor(MonsterBP->GeneratedClass);
	SpawnMonster->SetActorLocation(MonSpawnLocation);*/

}

void AMyGameModeBase::SpawnMonster(int oid, float x, float y, float z)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoFail = true;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = NULL;
	SpawnInfo.bDeferConstruction = false;
	// 몬스터 스폰 위치(일단 한마리 위치만 테스트용으로)
	FVector MonSpawnLocation = { x,y,z };
	// 몬스터 스폰 코드
	AMyMonster* SpawnMonster = GetWorld()->SpawnActor<AMyMonster>(MonToSpawn, MonSpawnLocation, FRotator::ZeroRotator, SpawnInfo); // 이렇게 하면 블프에서 구현해놓은 AI 구동이 안된다
	SpawnMonster->SetObjectID(oid);
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
	auto bossGolem = GetWorld()->SpawnActor<AMyBossGolem>(DefaultPawnClass, SpawnLocation, FRotator::ZeroRotator, SpawnInfo);
}

void AMyGameModeBase::SpawnPlayer(int oid, float x, float y, float z)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoFail = true;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = NULL;
	SpawnInfo.bDeferConstruction = false;
	FVector SpawnLocation = { 14410.0f, 77670.0f, -450.0f };
	auto MyChar = GetWorld()->SpawnActor<AMyCharacter>(DefaultPawnClass, SpawnLocation, FRotator::ZeroRotator, SpawnInfo);
	MyChar->SetID(oid);
}

void AMyGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ProcessEvent();
	// TEST
	/*
	static int aa = 0;
	APlayerController * NewPlayer = UGameplayStatics::CreatePlayer(GetWorld(), 1, true);
	FTransform PlayerSpawnTrans(FRotator::ZeroRotator, FVector(14410.0f, 77670.0f, -450.0f), FVector(1.0f, 1.0f, 1.0f));
	// 플레이어 스폰 코드
	RestartPlayerAtTransform(NewPlayer, PlayerSpawnTrans);
	aa++;
	Sleep(1000);
	*/	
}


void AMyGameModeBase::ProcessEvent() {
	net.eventLock.lock();
	if (net.eventQue.empty()) {
		net.eventLock.unlock();
		return;
	}
	auto ev = net.eventQue.front();
	net.eventLock.unlock();
	// net.eventQue.pop();

	switch (ev.type) {
	case sc_enter_obj: {
		if (ev.oid < NPC_ID_START) {
			
			//static int aa = 1;
			// 플레이어 캐릭터 스폰
			// SpawnMonster(ev.oid, ev.pos.x, ev.pos.y, ev.pos.z);	// 임시로 몬스터 사용
			//APlayerController * NewPlayer = UGameplayStatics::CreatePlayer(GetWorld(), ev.oid, true);
			//FTransform PlayerSpawnTrans(FRotator::ZeroRotator, FVector(14410.0f, 77670.0f, -450.0f), FVector(1.0f, 1.0f, 1.0f));
			// 플레이어 스폰 코드	
			// RestartPlayerAtTransform(NewPlayer, PlayerSpawnTrans);
			//aa++;
			SpawnPlayer(ev.oid, ev.pos.x, ev.pos.y, ev.pos.z);
			
		}
		else if (ev.oid == 20000) {
			UE_LOG(LogTemp, Log, TEXT("Boss Spawn in GMB"));
			SpawnBoss(ev.oid, ev.pos.x, ev.pos.y, ev.pos.z);
		}
		else
			SpawnMonster();
		
		net.eventLock.lock();
		net.eventQue.pop();
		net.eventLock.unlock();
	}break;
	
	// case sc_update_obj:
	// 	/*
	// 	각 오브젝트 클래스에서 자신에게 온 패킷인지를 검사 
	// 	waitfor은 틱이 멈춰서 못쓸거같고 이벤트큐를 하나 더 만들어야하나
	// 	여기서 각 클래스에 Update 요청하고 끝난 후에 이게 종료되어야 하는데
	// 	일단 이벤트큐로 전달을 하면
	// 	*/
	// 	UE_LOG(LogTemp, Log, TEXT("Update in GMB"));
	// 	// WaitForSingleObject(net.test1, 1000);
	// 	if (ev.oid < NPC_ID_START) {
	// 
	// 	}
	// 	else {
	// 
	// 	}
	// 	net.eventQue.pop();
	// 	break;
	default:
		break;
	}
}