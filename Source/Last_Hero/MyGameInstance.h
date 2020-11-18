// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Last_Hero.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LAST_HERO_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UMyGameInstance();
	virtual void Init() override;

	FStreamableManager StreamableManager;

	// 캐릭터 외형 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerInfo)
		int CharTypeNum;
};
