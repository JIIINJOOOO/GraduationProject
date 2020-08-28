// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "MyGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class LAST_HERO_API AMyGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	AMyGameStateBase();

public:
	int32 GetMonsterKillScore() const;
	void SetGameCleared();
	bool IsGameCleared() const;

private:
	
	UPROPERTY(Transient)
	int32 MonsterKillScore;

	UPROPERTY(Transient)
	bool bGameCleared;

	
};
