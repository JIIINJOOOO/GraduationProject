// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "MyAIController.h"
#include "MyBossGolem.h"
#include "MyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_TurnToTarget.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn");

}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto MyGolem = Cast<AMyBossGolem>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == MyGolem)
		return EBTNodeResult::Failed;
	//MyGolem->setIsTurningToTarget(false);

	auto Target = Cast<AMyCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(AMyAIController::TargetKey));
	if (nullptr == Target)
		return EBTNodeResult::Failed;
	


	FVector LookVector = Target->GetActorLocation() - MyGolem->GetActorLocation();
	LookVector.Z = 0.0f;
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	/*FTimerHandle Handle;
	FTimerDelegate TimerDel;
	TimerDel.BindUFunction(this, FName("ExecuteTask"), &OwnerComp, MyGolem);*/
	MyGolem->SetActorRotation(FMath::RInterpTo(MyGolem->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), 10.0f));
	bool bIsAttacking = OwnerComp.GetBlackboardComponent()->GetValueAsBool(AMyAIController::IsAttackingKey);
	//if (bIsAttacking == true /*&& (LookVector != Target->GetActorLocation() - MyGolem->GetActorLocation())*/)
	if (OwnerComp.GetBlackboardComponent()->GetValueAsRotator(AMyAIController::TargetRotKey) != FRotator(0.0f,0.0f,0.0f))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsTurningKey, true); 
		//OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsAttackingKey, false);
	}
	else
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsTurningKey, false); // 이때가 true일때
		//OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsAttackingKey, true); // 이때가 true일때
	}


	//MyGolem->setIsTurningToTarget(true);

	return EBTNodeResult::Succeeded;
}

void UBTTask_TurnToTarget::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	/*auto MyGolem = Cast<AMyBossGolem>(OwnerComp.GetAIOwner()->GetPawn());
	MyGolem->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
	});*/
	
	//if (!IsAttacking)
	//{
	//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsTurningKey, true); // 이때가 true일때
	//}
	if (!OwnerComp.GetBlackboardComponent()->GetValueAsBool(AMyAIController::IsTurningKey))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}


//void UBTTask_TurnToTarget::OnTaskFinished(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTNodeResult::Type TaskResult)
//{
//	//Super::OnTaskFinished(OwnerComp, NodeMemory, EBTNodeResult::Succeeded);
//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsAttackingKey, true); // 이때가 true일때
//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsTurningKey, false); // 이때가 true일때
//}

