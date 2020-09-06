// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Last_Hero.h"
#include "MyAIController.h"
#include "GameFramework/Character.h"
#include "MyBossGolem.generated.h"


//USTRUCT(Atomic,BlueprintType)
//struct NATIVETOBP_API FBoneInfo
//{
//	GENERATED_BODY()
//public:
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//		
//};
DECLARE_MULTICAST_DELEGATE(FOnAttackEndDelegate);

UCLASS()
class LAST_HERO_API AMyBossGolem : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyBossGolem();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	UFUNCTION()
		void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION(BlueprintCallable)
		static void BreakLegsCpp(AActor* Object);
	// Ice Spear
	UFUNCTION(BlueprintImplementableEvent) // CreateSpear custom event
		void CreateSpear();
	UFUNCTION(BlueprintImplementableEvent) 
		void ThrowSpear();
	UFUNCTION(BlueprintImplementableEvent) 
		void ChargeSpear();
	// ChargeSpear loop start,end 
	UFUNCTION(BlueprintCallable)
		void ChargeSpearLoopStart();
	UFUNCTION(BlueprintCallable)
		void ChargeSpearLoopEnd();
	// Stomping
	UFUNCTION(BlueprintImplementableEvent)
		void ChargeStomping();
	UFUNCTION(BlueprintImplementableEvent)
		void StompDamage();
	// ChargeStomping loop start,end 
	UFUNCTION(BlueprintCallable)
		void ChargeStompingLoopStart();
	UFUNCTION(BlueprintCallable)
		void ChargeStompingLoopEnd();
	
	


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BoneBreak) 
		TMap<FName, int32> BoneMap; // <본이름,피격횟수>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BoneBreak)
		bool IsDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fight)
		float Distance;
	// 보스가 플레이어를 발견 했는지 -> 처음 발견하면 true
	// -> 처음 발견했을때 시네마틱 / 포효
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fight)
		bool IsDetectInit;
	// 보스 발견 거리 안에 플레이어가 있는지 
	// -> 한번 플레이어를 발견한 상태인데 플레이어가 발견 거리말고 멀어지면
	//	 천천히 플레이어 쪽으로 다가가거나 longrange 어택
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fight)
		bool IsPlayerInDetRange;

	// bool var for broken parts
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BoneBreak)
		bool IsBreakLeftArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BoneBreak)
		bool IsBreakLeftLeg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BoneBreak)
		bool IsHalfBreakLeftArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BoneBreak)
		bool IsBreakRightArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BoneBreak)
		bool IsBreakRightLeg;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BoneBreak)
		bool IsHalfBreakRightArm;

	// Target Look Vector for set physics linear vel
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Fight)
		FVector TargetLookVec;
	// Ice Spear
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Fight)
		bool IsCreatingSpear;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Fight)
		bool IsChargingSpear;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Fight)
		FVector OrgScale;
	// Pattern Bool
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Fight)
		bool IsRushing;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Fight)
		bool IsStomping;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = Fight)
		bool IsWalkingAtk;


	void Attack_CloseRange();
	void Attack_LongRange();
	FOnAttackEndDelegate OnAttackEnd;


	// c++ functions
	bool getIsAttacking();
	bool getIsDown();
	bool getIsFalling();
	GOLEM_ANIM_MONTAGE getRndAtkMtg();
	void Launcher();
	void Launcher_Backward();
	void setGroundFrictionZero();
	GOLEM_ANIM_MONTAGE setRandomAttackMontage(GOLEM_ANIM_MONTAGE Min, GOLEM_ANIM_MONTAGE Max);

	// c++ variables
	FTimerHandle TimerHandle;
	GOLEM_ANIM_MONTAGE RndAtkMtg; // - Random Attack Pattern
	
	float LaunchForce;



private:
	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Fight, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BoneBreak, Meta = (AllowPrivateAccess = true))
		bool IsFalling;
	
	UPROPERTY()
		class UBossGolemAnimInstance* GolemAnim;

	// c++ variables
	bool IsDownInit;
};
