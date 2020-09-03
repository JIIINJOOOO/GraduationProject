// Fill out your copyright notice in the Description page of Project Settings.


#include "BossGolemAnimInstance.h"
#include "MyBossGolem.h"

UBossGolemAnimInstance::UBossGolemAnimInstance()
{
	CurrentPawnSpeed = 0.0f;
	// Standing State
	// - Close Range
	static ConstructorHelpers::FObjectFinder<UAnimMontage> NORMAL_ATTACK_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Golem_Mesh/Montage/Golem_Attack_Anim_Montage.Golem_Attack_Anim_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> HANDCLAP_ATTACK_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Golem_Mesh/Montage/Golem_HandClap_Anim_Montage.Golem_HandClap_Anim_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SWEEP_ATTACK_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Golem_Mesh/Montage/Golem_Sweep_Anim_Montage.Golem_Sweep_Anim_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> PUNCH_ATTACK_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Golem_Mesh/Montage/Golem_Punch_Anim_Montage.Golem_Punch_Anim_Montage")); // 애니메이션 똑같은데 조금 빠른 어택 애니메이션
	static ConstructorHelpers::FObjectFinder<UAnimMontage> STOMP_ATTACK_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Golem_Mesh/Montage/Golem_Stomping_Anim_Montage.Golem_Stomping_Anim_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> WALKING_ATTACK_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Golem_Mesh/Montage/Golem_Walking_Attack_Anim_Montage.Golem_Walking_Attack_Anim_Montage"));

	// - Long Range
	static ConstructorHelpers::FObjectFinder<UAnimMontage> RUSH_ATTACK_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Golem_Mesh/Montage/Golem_Rush_Anim_Montage.Golem_Rush_Anim_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> THROW_STONE_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Golem_Mesh/Montage/Golem_ThrowStone_Anim_Montage.Golem_ThrowStone_Anim_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> THROW_SPEAR_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Golem_Mesh/Montage/Golem_Spear_Montage.Golem_Spear_Montage"));



	// Down State
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DOWN_ATTACK_1_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Golem_Mesh/Montage/DownState/Golem_DownAttack1_Anim_Montage.Golem_DownAttack1_Anim_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DOWN_ATTACK_2_MONTAGE(TEXT("/Game/Game/Mesh/Monster/Monster_Golem_Mesh/Montage/DownState/Golem_DownAttack2_Anim_Montage.Golem_DownAttack2_Anim_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> GOLEM_BROKENLEG_FALLING(TEXT("/Game/Game/Mesh/Monster/Monster_Golem_Mesh/Montage/DownState/Golem_FallDown_Anim_Montage.Golem_FallDown_Anim_Montage"));


	// Stand State
	// - Close Range
	if (NORMAL_ATTACK_MONTAGE.Succeeded())
	{
		NormalAttackMontage = NORMAL_ATTACK_MONTAGE.Object;
	}
	if (HANDCLAP_ATTACK_MONTAGE.Succeeded())
	{
		HandClapAttackMontage = HANDCLAP_ATTACK_MONTAGE.Object;
	}
	if (SWEEP_ATTACK_MONTAGE.Succeeded())
	{
		SweepAttackMontage = SWEEP_ATTACK_MONTAGE.Object;
	}
	if (PUNCH_ATTACK_MONTAGE.Succeeded())
	{
		PunchAttackMontage = PUNCH_ATTACK_MONTAGE.Object;
	}
	if (STOMP_ATTACK_MONTAGE.Succeeded())
	{
		StompAttackMontage = STOMP_ATTACK_MONTAGE.Object;
	}
	if (WALKING_ATTACK_MONTAGE.Succeeded())
	{
		WalkingAttackMontage = WALKING_ATTACK_MONTAGE.Object;
	}
	// - Long Range
	if (RUSH_ATTACK_MONTAGE.Succeeded())
	{
		RushAttackMontage = RUSH_ATTACK_MONTAGE.Object;
	}
	if (THROW_STONE_MONTAGE.Succeeded())
	{
		ThrowStoneMontage = THROW_STONE_MONTAGE.Object;
	}
	if (THROW_SPEAR_MONTAGE.Succeeded())
	{
		ThrowSpearMontage = THROW_SPEAR_MONTAGE.Object;
	}

	// Down State
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

void UBossGolemAnimInstance::PlayGolemMontage(GOLEM_ANIM_MONTAGE select)
{
	switch (select)
	{
	// - Close Range
	case NORMAL_ATTACK:
		Montage_Play(NormalAttackMontage, 1.0f);
		break;
	case HANDCLAP_ATTACK:
		Montage_Play(HandClapAttackMontage, 1.0f);
		break;
	case SWEEP_ATTACK:
		Montage_Play(SweepAttackMontage, 1.0f);
		break;
	case PUNCH_ATTACK:
		Montage_Play(PunchAttackMontage, 1.0f);
		break;
	case STOMP_ATTACK:
		Montage_Play(StompAttackMontage, 1.0f);
		break;
	case WALKING_ATTACK:
		Montage_Play(WalkingAttackMontage, 1.0f);
		break;
	// - Long Range
	case RUSH_ATTACK:
		Montage_Play(RushAttackMontage, 1.0f);
		break;
	case THROW_STONE:
		Montage_Play(ThrowStoneMontage, 1.0f);
		break;
	case THROW_SPEAR:
		Montage_Play(ThrowSpearMontage, 1.0f);
		break;
	// - Down State
	case DOWN_ATTACK_1:
		Montage_Play(DownAttack1Montage, 1.0f);
		break;
	case DOWN_ATTACK_2:
		Montage_Play(DownAttack2Montage, 1.0f);
		break;
	case BROKENLEG_FALLING:
		Montage_Play(GolemFallingDownMontage, 1.0f);
		break;
	}

}
void UBossGolemAnimInstance::JumpToThrowSpearMontageSection(FName section)
{
	ABCHECK(Montage_IsPlaying(ThrowSpearMontage));
	Montage_JumpToSection(section, ThrowSpearMontage);
}
void UBossGolemAnimInstance::AnimNotify_Launch()
{
	auto BossGolem = Cast<AMyBossGolem>(TryGetPawnOwner());
	BossGolem->Launcher();
}
void UBossGolemAnimInstance::AnimNotify_CreateSpear()
{
	auto BossGolem = Cast<AMyBossGolem>(TryGetPawnOwner());
	BossGolem->CreateSpear();
	/*OnCreateSpear.Broadcast();*/
}
void UBossGolemAnimInstance::AnimNotify_ThrowSpear()
{
	auto BossGolem = Cast<AMyBossGolem>(TryGetPawnOwner());
	BossGolem->ThrowSpear();
}
void UBossGolemAnimInstance::AnimNotify_ChargeSpear()
{
	auto BossGolem = Cast<AMyBossGolem>(TryGetPawnOwner());
	BossGolem->ChargeSpear();
}
//FName UBossGolemAnimInstance::GetThrowSpearMontageSectionName(int32 Section)
//{
//	/*ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 3), NAME_None);*/
//	return FName(*FString::Printf(TEXT("));
//}
//
//void UBossGolemAnimInstance::PlayNormalAttackMontage()
//{
//	Montage_Play(NormalAttackMontage, 1.0f);
//}
//
//void UBossGolemAnimInstance::PlayHandClapAttackMontage()
//{
//	Montage_Play(HandClapAttackMontage, 1.0f);
//
//}
//
//void UBossGolemAnimInstance::PlaySweepAttackMontage()
//{
//	Montage_Play(SweepAttackMontage, 1.0f);
//
//}
//
//void UBossGolemAnimInstance::PlayPunchAttackMontage()
//{
//	Montage_Play(PunchAttackMontage, 1.0f);
//
//}
//
//void UBossGolemAnimInstance::PlayStompAttackMontage()
//{
//	Montage_Play(StompAttackMontage, 1.0f);
//
//}
//
//void UBossGolemAnimInstance::PlayWalkingAttackMontage()
//{
//	Montage_Play(WalkingAttackMontage, 1.0f);
//}
//
//void UBossGolemAnimInstance::PlayRushAttackMontage()
//{
//	Montage_Play(RushAttackMontage, 1.0f);
//}
//
//void UBossGolemAnimInstance::PlayThrowStoneMontage()
//{
//	Montage_Play(ThrowStoneMontage, 1.0f);
//}
//
//void UBossGolemAnimInstance::PlayDownAttack_1_Montage()
//{
//	Montage_Play(DownAttack1Montage, 1.0f);
//}
//
//void UBossGolemAnimInstance::PlayDownAttack_2_Montage()
//{
//	Montage_Play(DownAttack2Montage, 1.0f);
//}
//
//void UBossGolemAnimInstance::PlayGolemFallingDownMontage()
//{
//	Montage_Play(GolemFallingDownMontage, 1.0f);
//}
