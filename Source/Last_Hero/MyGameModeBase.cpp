// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameModeBase.h"
#include "MyCharacter.h"
#include "MyMonster.h"
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
	net.SendPacket(&p);
	// recv(net.m_sock, net.recvBuf, BUFSIZE, 0);
	// net.ProcessPacket(net.recvBuf);
}

void AMyGameModeBase::PostLogin(APlayerController * NewPlayer)
{
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

void AMyGameModeBase::SpawnPlayer(int oid, float x, float y, float z)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.bNoFail = true;
	SpawnInfo.Owner = this;
	SpawnInfo.Instigator = NULL;
	SpawnInfo.bDeferConstruction = false;
	// ���� ���� ��ġ(�ϴ� �Ѹ��� ��ġ�� �׽�Ʈ������)
	FVector MonSpawnLocation = { x,y,z };
	// ���� ���� �ڵ�
	AMyCharacter* SpawnMonster = GetWorld()->SpawnActor<AMyCharacter>(MonToSpawn, MonSpawnLocation, FRotator::ZeroRotator, SpawnInfo); // �̷��� �ϸ� �������� �����س��� AI ������ �ȵȴ�
	// monsters.insert(oid, SpawnMonster);
	/*AActor* SpawnMonster = GetWorld()->SpawnActor(MonsterBP->GeneratedClass);
	SpawnMonster->SetActorLocation(MonSpawnLocation);*/

}

void AMyGameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ProcessEvent();
	return;
	// �ۼ��� �� packet ó���� ���⼭ ����

	if (net.GetStatus() != p_login) {
		return;
	}
	// char error_code;
	// int error_code_size = sizeof(error_code);
	// getsockopt(net.m_sock, SOL_SOCKET, SO_ERROR, &error_code, &error_code_size);
	// if (error_code < 0) return;
	// error_code = 0;
	ProcessEvent();
	return;
	//int ret = recv(net.m_sock, net.recvBuf, BUFSIZE, 0);
	//net.ProcessPacket(net.recvBuf);
	if (num < 5) {

		SpawnMonster();
		num++;
	}
	if (net.gmb.type == sc_enter_obj) {
		SpawnMonster();
	}
	net.gmb.type = NULL; 
	return;
	// auto& ev = net.gmb;
	// if (ret == WAIT_OBJECT_0) {
	// 	UE_LOG(LogTemp, Log, TEXT("type :: %d %d"), (int)ev.type, (int)ev.oid);
	// }
	// else return;
	// UE_LOG(LogTemp, Log, TEXT("type :: %d %d"), (int)ev.type, (int)ev.oid);
	if (net.gmb.type == sc_enter_obj) {
		if (net.gmb.oid < NPC_ID_START) {
			// Player Spawn
			// SpawnPlayer(net.gmb.oid, net.gmb.pos.x, net.gmb.pos.y, net.gmb.pos.z);
			// SpawnMonster(net.gmb.oid, net.gmb.pos.x, net.gmb.pos.y, net.gmb.pos.z);
			// SpawnMonster();
			// FTransform spawnPos{ FVector(ev.pos.x, ev.pos.y, ev.pos.z) };
			// AController newPlayer;
			// RestartPlayerAtTransform(&newPlayer, spawnPos);
		}
		else if (net.gmb.oid < NPC_ID_START + MAX_MONSTER) {
			SpawnMonster(net.gmb.oid, net.gmb.pos.x, net.gmb.pos.y, net.gmb.pos.z);
			// SpawnPlayer(net.gmb.oid, net.gmb.pos.x, net.gmb.pos.y, net.gmb.pos.z);
			// APlayerController aa;
			// PostLogin(&aa);
			// auto& ev = net.gmb; 1.4
			// SpawnMonster(ev.oid, ev.pos.x, ev.pos.y, ev.pos.z);
			// FTransform spawnPos{ FVector(ev.pos.x, ev.pos.y, ev.pos.z) };
			// AController newPlayer;
			// RestartPlayerAtTransform(&newPlayer, spawnPos);
		}

		net.gmb.type = -1;
	}
	if (net.gmb.type == sc_update_obj) {

	}
	CS_MOVE p;
	p.destination = {};
	p.size = sizeof(p);
	p.type = move_packet;
	net.SendPacket(&p);
}

DWORD WINAPI AMyGameModeBase::GameThread(LPVOID arg) {
	auto gmb = reinterpret_cast<AMyGameModeBase*>(arg);

	while (true) {
		int ret = WaitForSingleObject(net.hEvent, INFINITE);

		if (ret == WAIT_OBJECT_0) { 
			if (net.gmb.type == sc_enter_obj) {
				//if (net.gmb.oid < NPC_ID_START) {

				
				// gmb->SpawnMonster(net.gmb.oid, net.gmb.pos.x, net.gmb.pos.y, net.gmb.pos.z);
			//	}
			}
		}
		else continue;
		
	}
	return 0;
}

void AMyGameModeBase::Test() {
	SpawnMonster();
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
	case sc_enter_obj:
		if (ev.oid < NPC_ID_START) {
			// �÷��̾� ĳ���� ����
			// SpawnMonster(ev.oid, ev.pos.x, ev.pos.y, ev.pos.z);	// �ӽ÷� ���� ���
		}
		else if (ev.oid == 20000) {/*�������� ���� ����*/}
		else 
			SpawnMonster();
		net.eventLock.lock();
		net.eventQue.pop();
		net.eventLock.unlock();
		break;
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