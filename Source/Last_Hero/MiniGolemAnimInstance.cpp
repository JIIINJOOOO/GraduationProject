// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniGolemAnimInstance.h"

UMiniGolemAnimInstance::UMiniGolemAnimInstance()
{
	speedSide_cpp = 0.0f;
	speedForward_cpp = 0.0f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HITREACTION_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Ice_Spirit/Ice_Spirit_Animation/Ice_Spirit_Hit_Montage.Ice_Spirit_Hit_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATK_1_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Ice_Spirit/Ice_Spirit_Animation/Ice_Spirit_Attack_Anim_Montage.Ice_Spirit_Attack_Anim_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATK_2_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Ice_Spirit/Ice_Spirit_Animation/Ice_Spirit_Attack2_Anim_Montage.Ice_Spirit_Attack2_Anim_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATK_3_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Ice_Spirit/Ice_Spirit_Animation/Ice_Spirit_Attack2_Anim_Montage.Ice_Spirit_Attack2_Anim_Montage"));

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

void UMiniGolemAnimInstance::PlayMiniGolemAnimMontage(UAnimMontage * Mtg)
{
	Montage_Play(Mtg, 1.0f);
}

void UMiniGolemAnimInstance::Hitreaction() {
	Montage_Play(HitReaction_Mtg, 1.0f);
}

void UMiniGolemAnimInstance::Attack1() {
	Montage_Play(Attack_1_Mtg, 1.0f);
}

void UMiniGolemAnimInstance::Attack2() {
	Montage_Play(Attack_2_Mtg, 1.0f);
}

void UMiniGolemAnimInstance::Attack3() {
	Montage_Play(Attack_3_Mtg, 1.0f);
}