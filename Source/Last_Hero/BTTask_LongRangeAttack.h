// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Last_Hero.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_LongRangeAttack.generated.h"

/**
 * 
 */
UCLASS()
class LAST_HERO_API UBTTask_LongRangeAttack : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UBTTask_LongRangeAttack();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	/*virtual void OnTaskFinished(UBehaviorTreeComponent & OwnerComp,
		uint8 * NodeMemory,
		EBTNodeResult::Type TaskResult);*/
private:
	bool IsAttacking = false;
};
