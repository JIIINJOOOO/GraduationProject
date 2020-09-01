// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAnimInstance.h"

UMyAnimInstance::UMyAnimInstance()
{
	speedSide_cpp = 0.0f;
	speedForward_cpp = 200.0f;
	isCrouched_cpp = false;
	Armed_Sword_cpp = false;
	Armed_Hammer_cpp = false;
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SWORD_INWARD_SLASH_MONTAGE(TEXT("/Game/Game/Mesh/player_anim/Mixamo/Stable_Sword_Inward_Slash_Montage.Stable_Sword_Inward_Slash_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> JUMP_MONTAGE(TEXT("/Game/Game/Mesh/player_anim/Mixamo/Jump_Montage.Jump_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> OUT_SWORD_MONTAGE(TEXT("/Game/Game/Mesh/player_anim/sword_shiend/outWeapon_Montage.outWeapon_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> IN_SWORD_MONTAGE(TEXT("/Game/Game/Mesh/player_anim/sword_shiend/inWeapon_Montage.inWeapon_Montage")); 
	static ConstructorHelpers::FObjectFinder<UAnimMontage> DEATH_WAIT_MONTAGE(TEXT("/Game/Game/Mesh/player_anim/Hammer/Death_wait_Montage.Death_wait_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> OUT_HAMMER_MONTAGE(TEXT("/Game/Game/Mesh/player_anim/Hammer/outWeapon_Montage.outWeapon_Montage"));
	
	static ConstructorHelpers::FObjectFinder<UAnimMontage> IN_HAMMER_MONTAGE(TEXT("/Game/Game/Mesh/player_anim/Hammer/inWeapon_Montage.inWeapon_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> CLIMB_END_MONTAGE(TEXT("/Game/Game/Mesh/player_anim/General/ClimbEnd_Montage.ClimbEnd_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> STANDING_TURN_LEFT_MONTAGE(TEXT("/Game/Game/Mesh/player_anim/Mixamo/Standing_Turn_Left_90_Montage.Standing_Turn_Left_90_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> UNARMED_TURN_RIGHT_MONTAGE(TEXT("/Game/Game/Mesh/player_anim/Mixamo/Unarmed_Turn_Right_90_Montage.Unarmed_Turn_Right_90_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> WALK_BACKWARDS_MONTAGE(TEXT("/Game/Game/Mesh/player_anim/Mixamo/Walk_Backwards__1__Montage.Walk_Backwards__1__Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SWORDSHIELD_IDLE_MONTAGE(TEXT("/Game/Game/Mesh/player_anim/Mixamo/Sword_And_Shield_Block_Idle_Montage.Sword_And_Shield_Block_Idle_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SWORDSHIELD_IMPACT_1_MONTAGE(TEXT("/Game/Game/Mesh/player_anim/Mixamo/Sword_And_Shield_Impact__1__Montage.Sword_And_Shield_Impact__1__Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SWORDSHIELD_IMPACT_MONTAGE(TEXT("/Game/Game/Mesh/player_anim/Mixamo/Sword_And_Shield_Impact_Montage.Sword_And_Shield_Impact_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SHIELD_BLOCK_MONTAGE(TEXT("/Game/Game/Mesh/player_anim/Mixamo/Sword_And_Shield_Block_Idle_Montage.Sword_And_Shield_Block_Idle_Montage"));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> PICKUP_MONTAGE(TEXT("/Game/Game/Mesh/player_anim/General/Pickup_Montage.Pickup_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SHIELD_STRIKE_MONTAGE(TEXT("/Game/Game/Mesh/player_anim/sword_shiend/Shield_Strike_Montage.Shield_Strike_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BERSERKER_MONTAGE(TEXT("/Game/Game/Mesh/player_anim/Mixamo/Sword_And_Shield_Power_Up_Montage.Sword_And_Shield_Power_Up_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> STANDING_DIVE_FORWARD_MONTAGE(TEXT("/Game/Game/Mesh/player_anim/Mixamo/Standing_Dive_Forward_Montage.Standing_Dive_Forward_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> EVADE_MONTAGE(TEXT("/Game/Game/Mesh/player_anim/Mixamo/Evade_Montage.Evade_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> BRACED_HANG_TO_CROUCH_MONTAGE(TEXT("/Game/Game/Mesh/player_anim/Mixamo/Evade_Montage.Evade_Montage"));

	static ConstructorHelpers::FObjectFinder<UAnimMontage> SWORD_SLASH_1_MONTAGE(TEXT("/Game/Game/Mesh/player_anim/Mixamo/Sword_And_Shield_Slash_Montage.Sword_And_Shield_Slash_Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SWORD_SLASH_2_MONTAGE(TEXT("/Game/Game/Mesh/player_anim/Mixamo/Sword_And_Shield_Slash__1__Montage.Sword_And_Shield_Slash__1__Montage"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> SWORD_SLASH_3_MONTAGE(TEXT("/Game/Game/Mesh/player_anim/sword_shiend/Shield_Strike_Montage.Shield_Strike_Montage"));


	
	if (SWORD_INWARD_SLASH_MONTAGE.Succeeded())
	{
		Sword_Inward_Slash_Mtg = SWORD_INWARD_SLASH_MONTAGE.Object;
	}
	if (JUMP_MONTAGE.Succeeded())
	{
		Jump_Mtg = JUMP_MONTAGE.Object;
	}
	if (OUT_SWORD_MONTAGE.Succeeded())
	{
		Out_Sword_Mtg = OUT_SWORD_MONTAGE.Object;
	}
	if (IN_SWORD_MONTAGE.Succeeded())
	{
		In_Sword_Mtg = IN_SWORD_MONTAGE.Object;
	}
	if (DEATH_WAIT_MONTAGE.Succeeded())
	{
		Death_Wait_Mtg = DEATH_WAIT_MONTAGE.Object;
	}
	if (OUT_HAMMER_MONTAGE.Succeeded())
	{
		Out_Hammer_Mtg = OUT_HAMMER_MONTAGE.Object;
	}
	if (IN_HAMMER_MONTAGE.Succeeded())
	{
		In_Hammer_Mtg = IN_HAMMER_MONTAGE.Object;
	}
	if (CLIMB_END_MONTAGE.Succeeded())
	{
		Climb_End_Mtg = CLIMB_END_MONTAGE.Object;
	}
	if (STANDING_TURN_LEFT_MONTAGE.Succeeded())
	{
		Standing_Turn_Left_90_Mtg = STANDING_TURN_LEFT_MONTAGE.Object;
	}
	if (UNARMED_TURN_RIGHT_MONTAGE.Succeeded())
	{
		Unarmed_Turn_Right_90_Mtg = UNARMED_TURN_RIGHT_MONTAGE.Object;
	}
	if (WALK_BACKWARDS_MONTAGE.Succeeded())
	{
		Walk_Backwards_1_Mtg = WALK_BACKWARDS_MONTAGE.Object;
	}
	if (SWORDSHIELD_IDLE_MONTAGE.Succeeded())
	{
		SwordShield_Block_Idle_Mtg = SWORDSHIELD_IDLE_MONTAGE.Object;
	}
	if (SWORDSHIELD_IMPACT_1_MONTAGE.Succeeded())
	{
		SwordShield_Impact_1_Mtg = SWORDSHIELD_IMPACT_1_MONTAGE.Object;
	}
	if (SWORDSHIELD_IMPACT_MONTAGE.Succeeded())
	{
		SwordShield_Impact_Mtg = SWORDSHIELD_IMPACT_MONTAGE.Object;
	}
	if (SHIELD_BLOCK_MONTAGE.Succeeded())
	{
		Shield_Block_Mtg = SHIELD_BLOCK_MONTAGE.Object;
	}
	if (PICKUP_MONTAGE.Succeeded())
	{
		PickUp_Mtg = PICKUP_MONTAGE.Object;
	}
	if (BRACED_HANG_TO_CROUCH_MONTAGE.Succeeded())
	{
		Braced_Hang_To_Crouch_Mtg = SWORDSHIELD_IMPACT_MONTAGE.Object;
	}
	if (SHIELD_STRIKE_MONTAGE.Succeeded())
	{
		Shield_Strike_Mtg = SHIELD_STRIKE_MONTAGE.Object;
	}
	if (BERSERKER_MONTAGE.Succeeded())
	{
		SwordShield_Berserker_Mtg = BERSERKER_MONTAGE.Object;
	}
	if (STANDING_DIVE_FORWARD_MONTAGE.Succeeded())
	{
		Standing_Dive_Forward_Mtg = STANDING_DIVE_FORWARD_MONTAGE.Object;
	}
	if (EVADE_MONTAGE.Succeeded())
	{
		Evade_Mtg = EVADE_MONTAGE.Object;
	}
	if (SWORD_SLASH_1_MONTAGE.Succeeded())
	{
		Sword_Slash_Combo1_Mtg = SWORD_SLASH_1_MONTAGE.Object;
	}
	if (SWORD_SLASH_2_MONTAGE.Succeeded())
	{
		Sword_Slash_Combo2_Mtg = SWORD_SLASH_2_MONTAGE.Object;
	}
	if (SWORD_SLASH_3_MONTAGE.Succeeded())
	{
		Sword_Slash_Combo3_Mtg = SWORD_SLASH_3_MONTAGE.Object;
	}

}

void UMyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	auto Pawn = TryGetPawnOwner();
	if (!::IsValid(Pawn)) return;
	//IsDead = Cast<AMyCharacter>
	/*if (!IsDead)
	{
		CurrentPawnSpeed = Pawn->GetVelocity().Size();
	}*/
}

void UMyAnimInstance::PlayPlayerAnimMontage(UAnimMontage * Mtg)
{
	Montage_Play(Mtg, 1.0f);
}

//void UMyAnimInstance::AnimNotify_EnableMovement()
//{
//	// call Toggle Movement(Enable)
//}
//
//void UMyAnimInstance::AnimNotify_DissableMovement()
//{
//	// call Toggle Movement(Disable)
//}
//
//void UMyAnimInstance::AnimNotify_CheckHit()
//{
//	// call Check Hit(Can Block = true)
//}
//
//void UMyAnimInstance::AnimNotify_CheckunblockableHit()
//{
//	// call Check Hit(Can Block = false)
//
//}
//
//void UMyAnimInstance::AnimNotify_AttackMove()
//{
//	// call Attack Move( Magnitude = 300.0 )
//}
//
//void UMyAnimInstance::AnimNotify_MoveBack()
//{
//	// call Attack Move( Magnitude = -300.0 )
//}

//void UMyAnimInstance::PlayAttackMontage()
//{
//	ABCHECK(!IsDead);
//	Montage_Play(AttackMontage, 1.0f);
//
//}

//void UMyAnimInstance::JumpToAttackMontageSection(int32 NewSection)
//{
//	ABCHECK(!IsDead);
//	ABCHECK(Montage_IsPlaying(AttackMontage));
//	Montage_JumpToSection(GetAttackMontageSectionName(NewSection), AttackMontage);
//}
//
//void UMyAnimInstance::AnimNotify_AttackHitCheck()
//{
//	//ABLOG_S("Warning");
//	OnAttackHitCheck.Broadcast();
//}
//
//void UMyAnimInstance::AnimNotify_NextAttackCheck()
//{
//	OnNextAttackCheck.Broadcast();
//}
//
//FName UMyAnimInstance::GetAttackMontageSectionName(int32 Section)
//{
//	ABCHECK(FMath::IsWithinInclusive<int32>(Section, 1, 5), NAME_None);
//	return FName(*FString::Printf(TEXT("Attack%d"), Section));
//}
