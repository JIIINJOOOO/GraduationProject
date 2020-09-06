// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/BlackboardComponent.h"

const FName AMyAIController::HomePosKey(TEXT("HomePos"));
const FName AMyAIController::PatrolPosKey(TEXT("PatrolPos"));
const FName AMyAIController::TargetKey(TEXT("Target"));
//const FName AMyAIController::AttackTargetKey(TEXT("AttackTarget"));
const FName AMyAIController::DistanceKey(TEXT("Distance"));
const FName AMyAIController::IsTurningKey(TEXT("IsTurning"));
const FName AMyAIController::IsAttackingKey(TEXT("IsAttacking"));
const FName AMyAIController::TargetRotKey(TEXT("TargetRot"));
const FName AMyAIController::GolemRotKey(TEXT("GolemRot"));
const FName AMyAIController::IsLongRangePatternKey(TEXT("IsLongRangePattern"));






AMyAIController::AMyAIController()
{
	static ConstructorHelpers::FObjectFinder<UBlackboardData> BBObject(TEXT("/Game/Game/BluePrints/Boss_Golem/Boss_Golem_AI/BB_BossGolem.BB_BossGolem"));
	if (BBObject.Succeeded())
	{
		BBAsset = BBObject.Object;
	}

	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BTObject(TEXT("/Game/Game/BluePrints/Boss_Golem/Boss_Golem_AI/BT_BossGolem.BT_BossGolem"));
	if (BTObject.Succeeded())
	{
		BTAsset = BTObject.Object;
	}
}

void AMyAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);
	if (UseBlackboard(BBAsset, Blackboard))
	{
		Blackboard->SetValueAsVector(HomePosKey, InPawn->GetActorLocation());
		if (!RunBehaviorTree(BTAsset))
		{
			ABLOG(Error, TEXT("AIController couldn't run behavior tree!"));
		}
	}
}
