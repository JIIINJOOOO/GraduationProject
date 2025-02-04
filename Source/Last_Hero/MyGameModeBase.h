// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Last_Hero.h"
#include "GameFramework/GameModeBase.h"
#include "Network.h"
#include "MyGameModeBase.generated.h"

/**
 *
 */
UCLASS()
class LAST_HERO_API AMyGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AMyGameModeBase();
protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	void ProcessEvent3();
	void DisplayResultScene();

public:
	/*UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "NetworkMonster")
		UBlueprint* MonsterBP;*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "NetworkMonster")
		TSubclassOf<class AMyMonsterAIController> MonsterAIBP; // goblin
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "NetworkMonster")
		TSubclassOf<class AMyMonsterAIController> OtherMonstersAIBP; // other monsters : 다 mini_golem AI 컨트롤러 사용중
	UPROPERTY(EditAnywhere, Category = "NetworkMonster")
		TSubclassOf<class AMyMonster_Goblin> MonToSpawn;


protected:
	UFUNCTION()
		void SpawnMonster();
	UFUNCTION()
		void SpawnGoblin(const int& oid, float x, float y, float z);
	UFUNCTION()
		void SpawnCyclops(const int& oid, float x, float y, float z);
	UFUNCTION()
		void SpawnBeetle(const int& oid, float x, float y, float z);
	UFUNCTION()
		void SpawnMiniGolem(const int& oid, float x, float y, float z);
	UFUNCTION()
		void SpawnLazard(const int& oid, float x, float y, float z);
	UFUNCTION()
		void SpawnPlayer(int oid, float x, float y, float z, int o_type);
	UFUNCTION()
		void SpawnGolem(int oid, float x, float y, float z);
	UFUNCTION()
		void SpawnTroll(int oid, float x, float y, float z);

	
};
