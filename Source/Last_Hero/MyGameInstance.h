// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Last_Hero.h"
#include "Engine/GameInstance.h"
#include "Engine/StreamableManager.h"
#include "Network.h"
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
	virtual void Shutdown() override;
	bool Tick(float DeltaTime);
	FStreamableManager StreamableManager;
	FDelegateHandle TickDelegateHandle;	

	// ĳ���� ���� Ÿ��
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerInfo)
		int CharTypeNum;

};
