// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_LongRangeAttack.h"
#include "MyAIController.h"
#include "MyBossGolem.h"
#include "MyCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_LongRangeAttack::UBTTask_LongRangeAttack()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_LongRangeAttack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	auto BossGolem = Cast<AMyBossGolem>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == BossGolem)
		return EBTNodeResult::Failed;
	auto Target = Cast<AMyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));
	if(nullptr == Target)
		return EBTNodeResult::Failed;

	if(BossGolem->getIsDown())
		return EBTNodeResult::Failed;
	if(BossGolem->getIsFalling())
		return EBTNodeResult::Failed;


	FVector LookVector = Target->GetActorLocation() - BossGolem->GetActorLocation();
	FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(BossGolem->GetActorLocation(), Target->GetActorLocation());
	BossGolem->TargetLookVec = UKismetMathLibrary::GetForwardVector(LookRot);
	//BossGolem->TargetLookVec = LookVector.ForwardVector;


	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	BossGolem->SetActorRotation(TargetRot);
	BossGolem->Attack_LongRange();

	IsAttacking = true;
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsAttackingKey, true); // 이때가 true일때
	//OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsTurningKey, false); // 이때가 true일때
	BossGolem->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_LongRangeAttack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	//else if (!IsAttacking && (OwnerComp.GetBlackboardComponent()->GetValueAsRotator(AMyAIController::TargetRotKey) == FRotator(0.0f, 0.0f, 0.0f)))
	//{
	//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsAttackingKey, true);
	//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsTurningKey, false);
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//}
}
