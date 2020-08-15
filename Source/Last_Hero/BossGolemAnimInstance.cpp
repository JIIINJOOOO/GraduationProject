// Fill out your copyright notice in the Description page of Project Settings.


#include "BossGolemAnimInstance.h"

UBossGolemAnimInstance::UBossGolemAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> NORMAL_ATTACK_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Golem_Mesh/AnimMontage/Golem_Attack_Anim_Montage.Golem_Attack_Anim_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DOWN_ATTACK_1_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Golem_Mesh/AnimMontage/Golem_DownAttack1_Anim_Montage.Golem_DownAttack1_Anim_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DOWN_ATTACK_2_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Golem_Mesh/AnimMontage/Golem_DownAttack2_Anim_Montage.Golem_DownAttack2_Anim_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> GOLEM_BROKENLEG_FALLING(TEXT("/Game/Game/Mesh/Monster/Monster_Golem_Mesh/AnimMontage/Golem_Down_Anim_Montage.Golem_Down_Anim_Montage"));

	if (NORMAL_ATTACK_MONTAGE.Succeeded())
	{
		NormalAttackMontage = NORMAL_ATTACK_MONTAGE.Object;
	}
	if (DOWN_ATTACK_1_MONTAGE.Succeeded())
	{
		DownAttack1Montage = DOWN_ATTACK_1_MONTAGE.Object;
	}
	if (DOWN_ATTACK_2_MONTAGE.Succeeded())
	{
		DownAttack2Montage = DOWN_ATTACK_2_MONTAGE.Object;
	}
	if (GOLEM_BROKENLEG_FALLING.Succeeded())
	{
		GolemFallingDownMontage = GOLEM_BROKENLEG_FALLING.Object;
	}
}

void UBossGolemAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn)) return;
	//IsDead = Cast<AMyCharacter>
	//if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
	}
}

void UBossGolemAnimInstance::PlayNormalAttackMontage()
{
	Montage_Play(NormalAttackMontage, 1.0f);
}

void UBossGolemAnimInstance::PlayDownAttack_1_Montage()
{
	Montage_Play(DownAttack1Montage, 1.0f);
}

void UBossGolemAnimInstance::PlayDownAttack_2_Montage()
{
	Montage_Play(DownAttack2Montage, 1.0f);
}

void UBossGolemAnimInstance::PlayGolemFallingDownMontage()
{
	Montage_Play(GolemFallingDownMontage, 1.0f);
}
