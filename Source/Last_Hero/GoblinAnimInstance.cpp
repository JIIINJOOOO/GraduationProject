// Fill out your copyright notice in the Description page of Project Settings.


#include "GoblinAnimInstance.h"

UGoblinAnimInstance::UGoblinAnimInstance()
{
	speedSide_cpp = 0.0f;
	speedForward_cpp = 0.0f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> STANDING_BLOCK_IDLE_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Goblin_Mesh/Animations/Standing_Block_Idle_mixamo_com_Montage.Standing_Block_Idle_mixamo_com_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DEATH_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Goblin_Mesh/AnimMontage/Goblin_death_montage.Goblin_death_montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SLASH_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Goblin_Mesh/AnimMontage/Golblin_slash_Montage.Golblin_slash_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATK_BACKHAND_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Goblin_Mesh/Animations/Goblin_Melee_Attack_Backhand_mixamo_com_Montage.Goblin_Melee_Attack_Backhand_mixamo_com_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATK_DOWNWARD_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Goblin_Mesh/Animations/Goblin_Melee_Attack_Downward_mixamo_com_Montage.Goblin_Melee_Attack_Downward_mixamo_com_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> ATK_3COMBO_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Goblin_Mesh/Animations/Goblin_Melee_Combo_Attack_Ver__3_mixamo_com_Montage.Goblin_Melee_Combo_Attack_Ver__3_mixamo_com_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HITREACTION_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Goblin_Mesh/AnimMontage/Goblin_HitReaction_Montage.Goblin_HitReaction_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BLOCK_REACT_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Goblin_Mesh/Animations/Standing_Block_React_Large_mixamo_com_Montage.Standing_Block_React_Large_mixamo_com_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> GETTINGUP_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Goblin_Mesh/Animations/Getting_Up_mixamo_com_Montage.Getting_Up_mixamo_com_Montage"));

	if (STANDING_BLOCK_IDLE_MONTAGE.Succeeded())
	{
		Standing_Block_Idle_Mtg = STANDING_BLOCK_IDLE_MONTAGE.Object;
	}
	if (DEATH_MONTAGE.Succeeded())
	{
		Death_Mtg = DEATH_MONTAGE.Object;
	}
	if (SLASH_MONTAGE.Succeeded())
	{
		Slash_Mtg = SLASH_MONTAGE.Object;
	}
	if (ATK_BACKHAND_MONTAGE.Succeeded())
	{
		Attack_Backhand_Mtg = ATK_BACKHAND_MONTAGE.Object;
	}
	if (ATK_DOWNWARD_MONTAGE.Succeeded())
	{
		Attack_Downward_Mtg = ATK_DOWNWARD_MONTAGE.Object;
	}
	if (ATK_3COMBO_MONTAGE.Succeeded())
	{
		Attack_3Combo_Mtg = ATK_3COMBO_MONTAGE.Object;
	}
	if (HITREACTION_MONTAGE.Succeeded())
	{
		HitReaction_Mtg = HITREACTION_MONTAGE.Object;
	}
	if (BLOCK_REACT_MONTAGE.Succeeded())
	{
		Block_React_Mtg = BLOCK_REACT_MONTAGE.Object;
	}
	if (GETTINGUP_MONTAGE.Succeeded())
	{
		GettingUp_Mtg = GETTINGUP_MONTAGE.Object;
	}

}

void UGoblinAnimInstance::PlayGoblinAnimMontage(UAnimMontage * Mtg)
{
	Montage_Play(Mtg, 1.0f);
}
