// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Last_Hero.h"
#include "Animation/AnimInstance.h"
#include "LazardAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LAST_HERO_API ULazardAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	ULazardAnimInstance();
	void PlayLazardAnimMontage(UAnimMontage* Mtg);
public:
	UFUNCTION(BlueprintImplementableEvent)
		void AnimNotify_EnableMovement();
	UFUNCTION(BlueprintImplementableEvent)
		void AnimNotify_DissableMovement();
	UFUNCTION(BlueprintImplementableEvent)
		void AnimNotify_CheckHit();
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* HitReaction_Mtg;// get hit
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Attack_1_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Attack_2_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Attack_3_Mtg;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server, Meta = (AllowPrivateAccess = true))
		float speedSide_cpp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server, Meta = (AllowPrivateAccess = true))
		float speedForward_cpp;

public:
	// Animation play Func
	void Hitreaction();
	void Attack1();
	void Attack2();
	void Attack3();
};
