// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"

UMyAnimInstance::UMyAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	ConstructorHelpers::FObjectFinder<UAnimMontage> ATTACK_MONTAGE(TEXT("/Game/Game/Mesh/Player_Character/AnimMontage/ComboAttackMontage_c++"));
	if (ATTACK_MONTAGE.Succeeded())
	{
		AttackMontage = ATTACK_MONTAGE.Object;
	}
}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (::IsValid(Pawn))
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
	}
}

void UMyAnimInstance::PlayAttackMontage()
{
	Montage_Play(AttackMontage, 1.0f);

}

void UMyAnimInstance::JumpToAttackMontageSection(int32 NewSection)
{
	ABCHECK(Montage_IsPlaying(AttackMontage));
	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
}

void UMyAnimInstance::AnimNotify_AttackHitCheck()
{
	//ABLOG_S("Warning");
	OnAttackHitCheck.Broadcast();
}

void UMyAnimInstance::AnimNotify_NextAttackCheck()
{
	OnNextAttackCheck.Broadcast();
}

FName UMyAnimInstance::GetAttackMontageSectionName(int32 Section)
{
	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 5), NAME_None);
	return FName(*FString::Printf(TEXT("Attack%d"), Section));
}
