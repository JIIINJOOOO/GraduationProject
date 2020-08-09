// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "MyAIController.h"
#include "MyBossGolem.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_Attack::UBTTask_Attack()
{
	bNotifyTick = true;
	IsAttacking = false;
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto BossGolem = Cast<AMyBossGolem>(OwnerComp.GetAIOwner()->GetPawn());
	if (nullptr == BossGolem)
		return EBTNodeResult::Failed;


	BossGolem->Attack();
	
	IsAttacking = true;
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsAttackingKey, true); // �̶��� true�϶�
	//OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsTurningKey, false); // �̶��� true�϶�
	BossGolem->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
	});

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking && (OwnerComp.GetBlackboardComponent()->GetValueAsRotator(AMyAIController::TargetRotKey) != FRotator(0.0f,0.0f,0.0f)))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsAttackingKey, false); // �̶��� true�϶�
		OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsTurningKey, true); // �̶��� true�϶�
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

//void UBTTask_Attack::OnTaskFinished(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTNodeResult::Type TaskResult)
//{
//	Super::OnTaskFinished(OwnerComp, NodeMemory, EBTNodeResult::Succeeded);
//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsAttackingKey, true); // �̶��� true�϶�
//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsTurningKey, false); // �̶��� true�϶�
//}
