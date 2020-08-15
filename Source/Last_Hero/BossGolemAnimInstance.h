// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Last_Hero.h"
#include "Animation/AnimInstance.h"
#include "BossGolemAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class LAST_HERO_API UBossGolemAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UBossGolemAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void PlayNormalAttackMontage();
	void PlayDownAttack_1_Montage();
	void PlayDownAttack_2_Montage();
	void PlayGolemFallingDownMontage();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* NormalAttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* DownAttack1Montage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* DownAttack2Montage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* GolemFallingDownMontage;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;
};
