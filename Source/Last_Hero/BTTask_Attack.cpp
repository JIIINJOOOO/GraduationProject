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
	if(BossGolem->getIsFalling())
		return EBTNodeResult::Failed;

	BossGolem->Attack_CloseRange();
	
	IsAttacking = true;
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsAttackingKey, true); // 이때가 true일때
	//OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsTurningKey, false); // 이때가 true일때
	BossGolem->OnAttackEnd.AddLambda([this]() -> void {
		IsAttacking = false;
	});
	/*GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]() {
		bCanAttackAgain = true;
	}, 2.0f, 1);*/

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (!IsAttacking && (OwnerComp.GetBlackboardComponent()->GetValueAsRotator(AMyAIController::TargetRotKey) != FRotator(0.0f,0.0f,0.0f))) // 플레이어 로테이션 바뀌었을때
	{
		/*if (bCanAttackAgain)
		{*/
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsAttackingKey, false);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsTurningKey, true);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		//}
	}
	else if (!IsAttacking && (OwnerComp.GetBlackboardComponent()->GetValueAsRotator(AMyAIController::TargetRotKey) == FRotator(0.0f, 0.0f, 0.0f))) // 플레이어 로테이션 안바뀌었을때
	{
		/*if (bCanAttackAgain)
		{*/
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsAttackingKey, true);
			OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsTurningKey, false);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		//}
	}

}

//void UBTTask_Attack::OnTaskFinished(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTNodeResult::Type TaskResult)
//{
//	Super::OnTaskFinished(OwnerComp, NodeMemory, EBTNodeResult::Succeeded);
//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsAttackingKey, true); // 이때가 true일때
//	OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsTurningKey, false); // 이때가 true일때
//}
