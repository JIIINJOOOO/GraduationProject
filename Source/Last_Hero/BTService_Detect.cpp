#include "BTService_Detect.h"
#include "MyAIController.h"
#include "MyBossGolem.h"
#include "MyCharacter.h"
#include "Engine/EngineTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
	/*static ConstructorHelpers::FObjectFinder<UBlueprint> BPChar(TEXT("/Game/Game/BluePrints/player/BP_player"));
	BPCharacter = BPChar.Object;*/
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;

	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 2000.0f;
	float RecogRadius = 3000.0f; // 처음 발견하고 나서만 인지 영역 생성

	if (nullptr == World) return;

	auto BossGolem = Cast<AMyBossGolem>(OwnerComp.GetAIOwner()->GetPawn());

	TArray<FOverlapResult> OverlapResults;

	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = false;
	if (BossGolem->IsDetectInit == false)
	{
		bResult = World->OverlapMultiByChannel(
			OverlapResults,
			Center,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel2,
			FCollisionShape::MakeSphere(DetectRadius),
			CollisionQueryParam
		);
	}
	else // 이미 발견한 상태일 때
	{
		bResult = World->OverlapMultiByChannel(
			OverlapResults,
			Center,
			FQuat::Identity,
			ECollisionChannel::ECC_GameTraceChannel2,
			FCollisionShape::MakeSphere(RecogRadius),
			CollisionQueryParam
		);
	}


	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			AMyCharacter* MyCharacter = Cast<AMyCharacter>(OverlapResult.GetActor());
			//AActor* AttackTarget = OverlapResult.GetActor();
			//auto MyCharacter = OverlapResult.GetActor();

			if (MyCharacter && MyCharacter->GetController()->IsPlayerController())
				//if (MyCharacter && MyCharacter->GetName() == "BP_Player_C_0")
			{
				//OwnerComp.GetBlackboardComponent()->SetValueAsFloat(AMyAIController::DistanceKey, Dist);
				/*if (Dist <= 200.0f)
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMyAIController::AttackTargetKey, MyCharacter);
				}
				else
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMyAIController::AttackTargetKey, nullptr);
				}*/
				if (BossGolem->IsDetectInit == false) // 처음 플레이어를 발견한거면
				{
					BossGolem->IsDetectInit = true;
				}
				float Dist = MyCharacter->GetDistanceTo(ControllingPawn);
				BossGolem->Distance = Dist;
				OwnerComp.GetBlackboardComponent()->SetValueAsRotator(AMyAIController::GolemRotKey, BossGolem->GetActorRotation());

				//OwnerComp.GetBlackboardComponent()->SetValueAsFloat(AMyAIController::DistanceKey, Dist);
				if (!BossGolem->getIsFalling())
				{
					if (Dist <= 900.0f) // Close Range
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsFloat(AMyAIController::DistanceKey, Dist);
						//OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsAttackingKey, true); // 이때가 true일때
						FVector LookVector = MyCharacter->GetActorLocation() - BossGolem->GetActorLocation();
						LookVector.Z = 0.0f;
						FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
						if (BossGolem->getIsAttacking() == true)
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsRotator(AMyAIController::TargetRotKey, TargetRot);

						}
						float result = FMath::Abs(BossGolem->GetActorRotation().Yaw - TargetRot.Yaw);
						if (result <= 20.0f)
						{
							OwnerComp.GetBlackboardComponent()->SetValueAsRotator(AMyAIController::TargetRotKey, FRotator(0.0f, 0.0f, 0.0f));
							OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsTurningKey, false);
							OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsAttackingKey, true);
						}

					}
					else	// Long Range
					{
						OwnerComp.GetBlackboardComponent()->SetValueAsFloat(AMyAIController::DistanceKey, 10000.0f);
						OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsAttackingKey, false);
						OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsTurningKey, false);
						if (!BossGolem->getIsDown())
							OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsLongRangePatternKey, FMath::RandRange(0, 1));
						else
							OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsLongRangePatternKey, false);
					}
				}





				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMyAIController::TargetKey, MyCharacter);


				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
				//DrawDebugSphere(World, Center, RecogRadius, 16, FColor::Green, false, 0.2f);

				DrawDebugPoint(World, MyCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), MyCharacter->GetActorLocation(), FColor::Blue, false, 0.27f);
				return;
			}
		}
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMyAIController::TargetKey, nullptr);

	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
	DrawDebugSphere(World, Center, RecogRadius, 16, FColor::Yellow, false, 0.2f);
}


//#include "BTService_Detect.h"
//#include "MyAIController.h"
//#include "MyBossGolem.h"
//#include "MyCharacter.h"
//#include "Engine/EngineTypes.h"
//#include "BehaviorTree/BlackboardComponent.h"
//#include "DrawDebugHelpers.h"
//
//UBTService_Detect::UBTService_Detect()
//{
//	NodeName = TEXT("Detect");
//	Interval = 1.0f;
//	/*static ConstructorHelpers::FObjectFinder<UBlueprint> BPChar(TEXT("/Game/Game/BluePrints/player/BP_player"));
//	BPCharacter = BPChar.Object;*/
//}
//
//void UBTService_Detect::TickNode(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
//{
//	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
//
//	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
//	if (nullptr == ControllingPawn) return;
//
//	UWorld* World = ControllingPawn->GetWorld();
//	FVector Center = ControllingPawn->GetActorLocation();
//	float DetectRadius = 2000.0f;
//	float RecogRadius = 4000.0f; // 처음 발견하고 나서만 인지 영역 생성
//
//	if (nullptr == World) return;
//
//	auto BossGolem = Cast<AMyBossGolem>(OwnerComp.GetAIOwner()->GetPawn());
//
//	TArray<FOverlapResult> OverlapResults;
//
//	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
//	bool bResult = false;
//	if (BossGolem->IsDetectInit == false)
//	{
//		bResult = World->OverlapMultiByChannel(
//			OverlapResults,
//			Center,
//			FQuat::Identity,
//			ECollisionChannel::ECC_GameTraceChannel2,
//			FCollisionShape::MakeSphere(DetectRadius),
//			CollisionQueryParam
//		);
//	}
//	else // 이미 발견한 상태일 때
//	{
//		bResult = World->OverlapMultiByChannel(
//			OverlapResults,
//			Center,
//			FQuat::Identity,
//			ECollisionChannel::ECC_GameTraceChannel2,
//			FCollisionShape::MakeSphere(RecogRadius),
//			CollisionQueryParam
//		);
//	}
//
//
//	if (bResult)
//	{
//		for (auto const& OverlapResult : OverlapResults)
//		{
//			AMyCharacter* MyCharacter = Cast<AMyCharacter>(OverlapResult.GetActor());
//			//TMap<AMyCharacter*,float> Players_Dist;
//			//TMap<AMyCharacter*, float> Players;
//
//			/*AMyCharacter* Player = Cast<AMyCharacter>(OverlapResult.GetActor());
//
//			float Dist = 0.0f;
//			if (Player->GetController()->IsPlayerController())
//			{
//				Dist = Player->GetDistanceTo(ControllingPawn);
//				Players.Emplace(Cast<AMyCharacter>(OverlapResult.GetActor()), Dist);
//			}
//
//			Players.ValueSort([](float Dist_1, float Dist_2) {
//				return Dist_1 < Dist_2;
//			});
//			Dist = Players.begin().Value();
//			AMyCharacter* MyCharacter = Players.begin().Key();
//			*/
//			//AActor* AttackTarget = OverlapResult.GetActor();
//			//auto MyCharacter = OverlapResult.GetActor();
//			// 10/3 캐릭터들을 배열에 담아서 걔네 다 거리 비교해서 젤 가까운애를 타겟으로
//			if (MyCharacter && MyCharacter->GetController()->IsPlayerController())
//			//if ((Players.Num() != 0)/* && IsPlayer*/)
//				//if (MyCharacter && MyCharacter->GetName() == "BP_Player_C_0")
//			{
//				//OwnerComp.GetBlackboardComponent()->SetValueAsFloat(AMyAIController::DistanceKey, Dist);
//				/*if (Dist <= 200.0f)
//				{
//					OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMyAIController::AttackTargetKey, MyCharacter);
//				}
//				else
//				{
//					OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMyAIController::AttackTargetKey, nullptr);
//				}*/
//				if (BossGolem->IsDetectInit == false) // 처음 플레이어를 발견한거면
//				{
//					BossGolem->IsDetectInit = true;
//				}
//
//				/*bool IsInitDist = false;
//				float Dist = 0.0f;
//				float TempDist = 0.0f;
//				for (auto const& Player : Players)
//				{
//					if (!IsInitDist)
//					{
//						Dist = Player->GetDistanceTo(ControllingPawn);
//						IsInitDist = true;
//					}
//					else
//					{
//						TempDist = Player->GetDistanceTo(ControllingPawn);
//					}
//					if (TempDist < Dist)
//					{
//						Dist = TempDist;
//					}
//				}*/
//				// 10/3 3:02 여기까지함 -> 스왑로직 맞나 맨정신에 다시 생각해보자
//
//
//				float Dist = MyCharacter->GetDistanceTo(ControllingPawn);
//				BossGolem->Distance = Dist;
//				OwnerComp.GetBlackboardComponent()->SetValueAsRotator(AMyAIController::GolemRotKey, BossGolem->GetActorRotation());
//
//				//OwnerComp.GetBlackboardComponent()->SetValueAsFloat(AMyAIController::DistanceKey, Dist);
//				if (!BossGolem->getIsFalling())
//				{
//					if (Dist <= 900.0f) // Close Range
//					{
//						OwnerComp.GetBlackboardComponent()->SetValueAsFloat(AMyAIController::DistanceKey, Dist);
//						//OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsAttackingKey, true); // 이때가 true일때
//						FVector LookVector = MyCharacter->GetActorLocation() - BossGolem->GetActorLocation();
//						LookVector.Z = 0.0f;
//						FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
//						if (BossGolem->getIsAttacking() == true)
//						{
//							OwnerComp.GetBlackboardComponent()->SetValueAsRotator(AMyAIController::TargetRotKey, TargetRot);
//
//						}
//						float result = FMath::Abs(BossGolem->GetActorRotation().Yaw - TargetRot.Yaw);
//						if (result <= 20.0f)
//						{
//							OwnerComp.GetBlackboardComponent()->SetValueAsRotator(AMyAIController::TargetRotKey, FRotator(0.0f, 0.0f, 0.0f));
//							OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsTurningKey, false);
//							OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsAttackingKey, true);
//						}
//
//					}
//					else	// Long Range
//					{
//						OwnerComp.GetBlackboardComponent()->SetValueAsFloat(AMyAIController::DistanceKey, 10000.0f);
//						OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsAttackingKey, false);
//						OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsTurningKey, false);
//						if (!BossGolem->getIsDown())
//							OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsLongRangePatternKey, FMath::RandRange(0, 1));
//						else
//							OwnerComp.GetBlackboardComponent()->SetValueAsBool(AMyAIController::IsLongRangePatternKey, false);
//					}
//				}
//
//
//
//
//
//				OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMyAIController::TargetKey, MyCharacter);
//
//
//				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
//				//DrawDebugSphere(World, Center, RecogRadius, 16, FColor::Green, false, 0.2f);
//
//				DrawDebugPoint(World, MyCharacter->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
//				DrawDebugLine(World, ControllingPawn->GetActorLocation(), MyCharacter->GetActorLocation(), FColor::Blue, false, 0.27f);
//				return;
//			}
//		}
//	}
//	if (!BossGolem->IsDetectInit)
//		OwnerComp.GetBlackboardComponent()->SetValueAsObject(AMyAIController::TargetKey, nullptr);
//
//	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);
//	DrawDebugSphere(World, Center, RecogRadius, 16, FColor::Yellow, false, 0.2f);
//}