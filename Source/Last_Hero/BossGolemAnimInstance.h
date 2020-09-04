// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Last_Hero.h"
#include "Animation/AnimInstance.h"
#include "BossGolemAnimInstance.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnCreateSpearDelegate);
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

	void PlayGolemMontage(GOLEM_ANIM_MONTAGE select);
	void JumpToThrowSpearMontageSection(FName section);

public:
	/*FOnCreateSpearDelegate OnCreateSpear;*/
	// Stand Atk
	// - Close Range
	//void PlayNormalAttackMontage();
	//void PlayHandClapAttackMontage();
	//void PlaySweepAttackMontage();
	//void PlayPunchAttackMontage();
	//void PlayStompAttackMontage();
	//void PlayWalkingAttackMontage();
	//// - Long Range
	//void PlayRushAttackMontage();
	//void PlayThrowStoneMontage();



	//// Down Atk
	//void PlayDownAttack_1_Montage();
	//void PlayDownAttack_2_Montage();
	//void PlayGolemFallingDownMontage();
private:
	UFUNCTION()
		void AnimNotify_Launch();
	UFUNCTION()
		void AnimNotify_Launch_Backward();
	UFUNCTION()
		void AnimNotify_CreateSpear();
	UFUNCTION()
		void AnimNotify_ThrowSpear();
	UFUNCTION()
		void AnimNotify_ChargeSpear();
	//FName GetThrowSpearMontageSectionName(int32 Section);
private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character, Meta = (AllowPrivateAccess = true))
		float CurrentPawnSpeed;

	// Stand Atk
	// - Close Range
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* NormalAttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* HandClapAttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* SweepAttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* PunchAttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* StompAttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* WalkingAttackMontage;
	// - Long Range
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* RushAttackMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* ThrowStoneMontage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* ThrowSpearMontage;

	// Down Atk
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* DownAttack1Montage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* DownAttack2Montage;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* GolemFallingDownMontage;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;
};
