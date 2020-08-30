// Fill out your copyright notice in the Description page of Project Settings.


#include "LazardAnimInstance.h"

ULazardAnimInstance::ULazardAnimInstance()
{
	speedSide_cpp = 0.0f;
	speedForward_cpp = 0.0f;

	static ConstructorHelpers::FObjectFinder<UAnimMontage> HITREACTION_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Lazardman/anim/Lizarman_Hit_Montage.Lizarman_Hit_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATK_1_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Lazardman/anim/Lizarman_Attack_Montage.Lizarman_Attack_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATK_2_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Lazardman/anim/Lizarman_Attack2_Montage.Lizarman_Attack2_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATK_3_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Lazardman/anim/Lizarman_Attack3_Montage.Lizarman_Attack3_Montage"));

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

void ULazardAnimInstance::PlayLazardAnimMontage(UAnimMontage * Mtg)
{
	Montage_Play(Mtg, 1.0f);
}