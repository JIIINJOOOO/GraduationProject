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


public:
	/*UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "NetworkMonster")
		UBlueprint* MonsterBP;*/
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "NetworkMonster")
		UBlueprint* MonsterAIBP;
	UPROPERTY(EditAnywhere, Category = "NetworkMonster")
		TSubclassOf<class AMyMonster_Goblin> MonToSpawn;
	UPROPERTY(EditAnywhere, Category = "NetworkCharacter")
		TSubclassOf<class AMyCharacter> CharToSpawn;
	UPROPERTY(EditAnywhere, Category = "NetworkBoss")
		TSubclassOf<class AMyBossGolem> BossToSpawn;
	void ProcessEvent();
protected:
	UFUNCTION()
		void SpawnMonster();
	void SpawnMonster(int oid, float x, float y, float z);
	UFUNCTION()
		void SpawnPlayer(int oid, float x, float y, float z);
	void SpawnBoss(int oid, float x, float y, float z);
};
