// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Last_Hero.h"
#include "Network.h"
#include "GameFramework/Character.h"
#include "MyMonster.generated.h"

#define CYCLOPS_ID 11000
#define BEETLE_ID 12000
#define MINI_GOLEM_ID 13000
#define LAZARD_ID 14000

UCLASS()
class LAST_HERO_API AMyMonster : public ACharacter
{
	GENERATED_BODY()
private:
	// 몬스터 이름들 다 하드코딩해서 가져오고 호스트한테서 물리값, 좌표 등 받아와서 중계만 해보자
	FString monName1 = "BP_Cyclops";
	FString monName2 = "BP_Beetle_Warrior";
	FString monName3 = "Mini_golem";
	FString monName4 = "BP_Lazardman";

	int id;

	FRotator rotation;
	FVector velocity;
	float speed;
	bool isDead;
	int hp;
	int type;
	int deadCnt;
	bool isMoving;
	FVector netPos;
	high_resolution_clock::time_point deathTime;

public:
	// Sets default values for this character's properties
	AMyMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		FVector MonPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterInfo)
		float ServerMonHP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterInfo)
		float ServerDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterInfo)
		float ServerRecovery;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		int RandAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		bool ready_cpp;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsCyclops();
	bool IsBeetle();
	bool IsMiniGolem();
	bool IsLazard();

	void SetID(const int& new_id);
	void SetType(const int& mon_type);

	void CyclopsUpdate();
	void BeetleUpdate();
	void MiniGolemUpdate();
	void LazardUpdate();
};
