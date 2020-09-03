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
private:
	int id;

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
	UFUNCTION(BlueprintImplementableEvent) // CreateSpear custom event
		void CreateSpear();
	UFUNCTION(BlueprintImplementableEvent) // CreateSpear custom event
		void ThrowSpear();
	UFUNCTION(BlueprintImplementableEvent) // CreateSpear custom event
		void ChargeSpear();

	UFUNCTION(BlueprintCallable)
		void ChargeSpearLoopStart();
	UFUNCTION(BlueprintCallable)
		void ChargeSpearLoopEnd();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BoneBreak) 
		TMap<FName, int32> BoneMap; // <본이름,피격횟수>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BoneBreak)
		bool IsDown;

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
	// Arm-Weapon IsOverlapping var 
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = BoneBreak)
		bool IsOverlapping_RArm;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = BoneBreak)
		bool IsOverlapping_LArm;
	

	void Attack_CloseRange();
	void Attack_LongRange();
	FOnAttackEndDelegate OnAttackEnd;


	// c++ functions
	bool getIsAttacking();
	bool getIsDown();
	GOLEM_ANIM_MONTAGE getRndAtkMtg();
	void Launcher();
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
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = BoneBreak, Meta = (AllowPrivateAccess = true))
		bool IsFalling;
	
	UPROPERTY()
		class UBossGolemAnimInstance* GolemAnim;

	// c++ variables
	bool IsDownInit;

public:
	void SetID(const int& id);
};
