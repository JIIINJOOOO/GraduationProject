// Fill out your copyright notice in the Description page of Project Settings.


#include "BeetleAnimInstance.h"

UBeetleAnimInstance::UBeetleAnimInstance()
{
	speedSide_cpp = 0.0f;
	speedForward_cpp = 0.0f;


	static ConstructorHelpers::FObjectFinder<UAnimMontage> HITREACTION_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_BeetleWarrior/anim/Beetle_Warrior_Hit_Montage.Beetle_Warrior_Hit_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATK_1_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_BeetleWarrior/anim/Beetle_Warrior_Attack1_Montage.Beetle_Warrior_Attack1_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATK_2_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_BeetleWarrior/anim/Beetle_Warrior_Attack2_Montage.Beetle_Warrior_Attack2_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATK_3_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_BeetleWarrior/anim/Beetle_Warrior_Attack3_Montage.Beetle_Warrior_Attack3_Montage"));

	if (HITREACTION_MONTAGE.Succeeded())
	{
		HitReaction_Mtg = HITREACTION_MONTAGE.Object;
	}
	if (ATK_1_MONTAGE.Succeeded())
	{
		Attack_1_Mtg = ATK_1_MONTAGE.Object;
	}
	if (ATK_2_MONTAGE.Succeeded())
	{
		Attack_2_Mtg = ATK_2_MONTAGE.Object;
	}
	if (ATK_3_MONTAGE.Succeeded())
	{
		Attack_3_Mtg = ATK_3_MONTAGE.Object;
	}
}

void UBeetleAnimInstance::PlayBeetleAnimMontage(UAnimMontage * Mtg)
{
	Montage_Play(Mtg, 1.0f);
}

void UBeetleAnimInstance::Hitreaction() {
	Montage_Play(HitReaction_Mtg, 1.0f);
}

void UBeetleAnimInstance::Attack1() {
	Montage_Play(Attack_1_Mtg, 1.0f);
}

void UBeetleAnimInstance::Attack2() {
	Montage_Play(Attack_2_Mtg, 1.0f);
}

void UBeetleAnimInstance::Attack3() {
	Montage_Play(Attack_3_Mtg, 1.0f);
}