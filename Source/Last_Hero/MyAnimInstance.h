// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Last_Hero.h"
#include "Animation/AnimInstance.h"
#include "MyAnimInstance.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnNextAttackCheckDelegate);
DECLARE_MULTICAST_DELEGATE(FOnAttackHitCheckDelegate);


/**
 * 
 */
UCLASS()
class LAST_HERO_API UMyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UMyAnimInstance();
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	void PlayPlayerAnimMontage(UAnimMontage* Mtg);
	//void JumpToAttackMontageSection(int32 NewSection);

public:
	UFUNCTION(BlueprintImplementableEvent)
		void AnimNotify_EnableMovement();
	UFUNCTION(BlueprintImplementableEvent)
		void AnimNotify_DissableMovement();
	UFUNCTION(BlueprintImplementableEvent)
		void AnimNotify_CheckHit();
	UFUNCTION(BlueprintImplementableEvent)
		void AnimNotify_CheckunblockableHit();
	UFUNCTION(BlueprintImplementableEvent)
		void AnimNotify_AttackMove();
	UFUNCTION(BlueprintImplementableEvent)
		void AnimNotify_MoveBack();
//public:
//	FOnNextAttackCheckDelegate OnNextAttackCheck;
//	FOnAttackHitCheckDelegate OnAttackHitCheck;
//private:
//	UFUNCTION()
//		void AnimNotify_AttackHitCheck();
//	UFUNCTION()
//		void AnimNotify_NextAttackCheck();
//
//	FName GetAttackMontageSectionName(int32 Section);
public:
	// Animation Func
	void StableSwordInward();	// 암살
	void Jump();
	void OutSword();	// 
	void InSword();
	void Death();
	void OutHammer();
	void InHammer();
	void ClimbEnd();
	void StandingTurnLeft();
	void UnarmedTurnRight();
	void WalkBackwards();
	void SwordShieldIdle();	// 이거도 방어?
	void SwordShieldImpact1();	// 공격 받았을때 같은데
	void SwordShieldImpact();	// ???
	void ShieldBlock(); // 방어
	void Pickup();
	void ShieldStrike();	// 방패러 ㅊ;ㄴ,ㄴ가
	void Berserker();
	void StandingDibeForward();
	void Evade();
	void BracedHangToCrouch();

	void Move(float speedSide = 0.f, float speedForward = 200.f);

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server, Meta = (AllowPrivateAccess = true))
		float speedSide_cpp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server, Meta = (AllowPrivateAccess = true))
		float speedForward_cpp;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* PlayerAnimMontage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server, Meta = (AllowPrivateAccess = true))
		bool isCrouched_cpp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server, Meta = (AllowPrivateAccess = true))
		bool Armed_Sword_cpp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server, Meta = (AllowPrivateAccess = true))
		bool Armed_Hammer_cpp;
private: // Montages
	// Sword&Shield Montages
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Out_Sword_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* In_Sword_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Shield_Block_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* SwordShield_Impact_Mtg; // get hit
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* SwordShield_Impact_1_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* SwordShield_Block_Idle_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Shield_Strike_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* SwordShield_Berserker_Mtg;
	// Hammer Montages
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Out_Hammer_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* In_Hammer_Mtg;
	
	

	// Movement Montages
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Jump_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Stable_Sword_Inward_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Death_Wait_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Standing_Turn_Left_90_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Unarmed_Turn_Right_90_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
		UAnimMontage* PickUp_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Standing_Dive_Forward_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Evade_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Braced_Hang_To_Crouch_Mtg;

	// Climb Montages
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Climb_End_Mtg;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		UAnimMontage* Walk_Backwards_1_Mtg;

};
