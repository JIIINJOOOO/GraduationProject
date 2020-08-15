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
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BoneBreak) 
		TMap<FName, int32> BoneMap; // <본이름,피격횟수>
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = BoneBreak)
		bool IsDown;

	void Attack();
	FOnAttackEndDelegate OnAttackEnd;


	// c++ functions
	bool getIsAttacking();
	bool getIsDown();
	int getRndDownAtk();
	// random down attack
	int RndDownAtk;
private:
	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;
	
	UPROPERTY()
		class UBossGolemAnimInstance* GolemAnim;

	// c++ variables
	bool IsDownInit;
};
