// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Last_Hero.h"
#include "AIController.h"
#include "MyMonsterAIController.generated.h"

/**
 * 
 */
UCLASS()
class LAST_HERO_API AMyMonsterAIController : public AAIController
{
	GENERATED_BODY()
public:
	AMyMonsterAIController();

	virtual void OnPossess(APawn* aPawn) override;
	
};
