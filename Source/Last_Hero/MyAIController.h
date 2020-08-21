// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Last_Hero.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class LAST_HERO_API AMyAIController : public AAIController
{
	GENERATED_BODY()
public:
	AMyAIController();
	virtual void OnPossess(APawn* InPawn) override;

	static const FName HomePosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;
	//static const FName AttackTargetKey;
	static const FName DistanceKey;
	static const FName IsTurningKey;
	static const FName IsAttackingKey;
	static const FName TargetRotKey;
	static const FName GolemRotKey;
	static const FName IsLongRangePatternKey;





private:
	UPROPERTY()
		class UBehaviorTree* BTAsset;

	UPROPERTY()
		class UBlackboardData* BBAsset;
};
