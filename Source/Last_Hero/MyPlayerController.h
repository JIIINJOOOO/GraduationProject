// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Last_Hero.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LAST_HERO_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMyPlayerController();
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;
	virtual void Tick(float DeltaTime) override;
	bool isBattle;
	bool isInput;
};
