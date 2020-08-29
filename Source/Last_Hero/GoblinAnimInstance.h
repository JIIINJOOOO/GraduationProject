// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Last_Hero.h"
#include "Animation/AnimInstance.h"
#include "GoblinAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LAST_HERO_API UGoblinAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UGoblinAnimInstance();
	void PlayGoblinAnimMontage(UAnimMontage* Mtg);
public:
	UFUNCTION(BlueprintImplementableEvent)
		void AnimNotify_EnableMovement();
	UFUNCTION(BlueprintImplementableEvent)
		void AnimNotify_DissableMovement();
	UFUNCTION(BlueprintImplementableEvent)
		void AnimNotify_CheckHit();
private:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Standing_Block_Idle_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Death_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Slash_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Attack_Backhand_Mtg; // get hit
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Attack_Downward_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Attack_3Combo_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* HitReaction_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Block_React_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* GettingUp_Mtg;
private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server, Meta = (AllowPrivateAccess = true))
		float speedSide_cpp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server, Meta = (AllowPrivateAccess = true))
		float speedForward_cpp;
};
