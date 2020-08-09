// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsInAttackRange.h"
#include "MyCharacter.h"
#include "MyAIController.h"
#include "MyBossGolem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsInAttackRange::UBTDecorator_IsInAttackRange()
{
	NodeName = TEXT("CanAttack");
}

bool UBTDecorator_IsInAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
		return false;

	//auto Target = Cast<AMyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));
	//auto Target = OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey);
	//auto Target = OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey);
	auto Target = Cast<AMyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));
	if (nullptr == Target)
		return false;
	float Dist = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(AMyAIController::DistanceKey);

	
	bResult = (Target->GetDistanceTo(ControllingPawn) <= 900.0f/*ControllingPawn->GetFinalAttackRange()*/);


	/*float Dist = 0.0f;

	if(bResult)
		Dist = NewTarget->GetDistanceTo(ControllingPawn);*/
	
	return bResult;
}
