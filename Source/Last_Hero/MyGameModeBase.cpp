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
	
	// �α���â ���� ���� ������,��
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
	// �÷��̾� ���� ��ġ Ʈ������
	FTransform PlayerSpawnTrans(FRotator::ZeroRotator, FVector(14410.0f, 77670.0f, -450.0f), FVector(1.0f, 1.0f, 1.0f));
	// �÷��̾� ���� �ڵ�
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
	// ���� ���� ��ġ(�ϴ� �Ѹ��� ��ġ�� �׽�Ʈ������)
	FVector MonSpawnLocation = { 19266.0f,78928.0f,-440.0f };
	// ���� ���� �ڵ�
	AMyMonster* SpawnMonster = GetWorld()->SpawnActor<AMyMonster>(MonToSpawn, MonSpawnLocation, FRotator::ZeroRotator, SpawnInfo); // �̷��� �ϸ� �������� �����س��� AI ������ �ȵȴ�
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
	// ���� ���� ��ġ(�ϴ� �Ѹ��� ��ġ�� �׽�Ʈ������)
	FVector MonSpawnLocation = { x,y,z };
	// ���� ���� �ڵ�
	AMyMonster* SpawnMonster = GetWorld()->SpawnActor<AMyMonster>(MonToSpawn, MonSpawnLocation, FRotator::ZeroRotator, SpawnInfo); // �̷��� �ϸ� �������� �����س��� AI ������ �ȵȴ�
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
	// ���� ���� ��ġ(�ϴ� �Ѹ��� ��ġ�� �׽�Ʈ������)
	FVector SpawnLocation = { x,y,z };
	// ���� ���� �ڵ�
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
	// �÷��̾� ���� �ڵ�
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
			// �÷��̾� ĳ���� ����
			// SpawnMonster(ev.oid, ev.pos.x, ev.pos.y, ev.pos.z);	// �ӽ÷� ���� ���
			//APlayerController * NewPlayer = UGameplayStatics::CreatePlayer(GetWorld(), ev.oid, true);
			//FTransform PlayerSpawnTrans(FRotator::ZeroRotator, FVector(14410.0f, 77670.0f, -450.0f), FVector(1.0f, 1.0f, 1.0f));
			// �÷��̾� ���� �ڵ�	
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
	// 	�� ������Ʈ Ŭ�������� �ڽſ��� �� ��Ŷ������ �˻� 
	// 	waitfor�� ƽ�� ���缭 �����Ű��� �̺�Ʈť�� �ϳ� �� �������ϳ�
	// 	���⼭ �� Ŭ������ Update ��û�ϰ� ���� �Ŀ� �̰� ����Ǿ�� �ϴµ�
	// 	�ϴ� �̺�Ʈť�� ������ �ϸ�
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