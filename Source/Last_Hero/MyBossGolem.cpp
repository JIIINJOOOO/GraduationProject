// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBossGolem.h"
#include "BossGolemAnimInstance.h"

// Sets default values
AMyBossGolem::AMyBossGolem()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyMonster"));
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
	AIControllerClass = AMyAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	IsAttacking = false;
	IsRushing = false;
	IsWalkingAtk = false;
	IsStomping = false;
	IsDown = false;
	IsFalling = false;
	IsDetectInit = false;
	IsPlayerInDetRange = false;
}

void AMyBossGolem::Attack_CloseRange()
{
	if (IsAttacking) return;
	if (IsFalling) return;
	if (!IsDown)
	{
		RndAtkMtg = setRandomAttackMontage(CLOSE_RANGE_START, CLOSE_RANGE_END);
	}
	else
	{
		RndAtkMtg = setRandomAttackMontage(DOWN_START, DOWN_END);
		/*if (RndDownAtk == 0)
		{
			GolemAnim->PlayDownAttack_1_Montage();
		}
		else
		{
			GolemAnim->PlayDownAttack_2_Montage();
		}*/
	}
	GolemAnim->PlayGolemMontage(RndAtkMtg);
	//GolemAnim->PlayGolemMontage(STOMP_ATTACK);


	IsAttacking = true;
}

void AMyBossGolem::Attack_LongRange()
{
	if (IsAttacking) return;
	if (IsFalling) return;
	if (!IsDown)
	{
		RndAtkMtg = setRandomAttackMontage(LONG_RANGE_START, LONG_RANGE_END);
	}
	//GolemAnim->PlayGolemMontage(THROW_SPEAR);
	GolemAnim->PlayGolemMontage(RndAtkMtg);
	IsAttacking = true;
}


bool AMyBossGolem::getIsAttacking()
{
	return IsAttacking;
}

bool AMyBossGolem::getIsDown()
{
	return IsDown;
}

bool AMyBossGolem::getIsFalling()
{
	return IsFalling;
}

GOLEM_ANIM_MONTAGE AMyBossGolem::getRndAtkMtg()
{
	return RndAtkMtg;
}

void AMyBossGolem::Launcher()
{
	float JumpForce = 1.0f;
	FVector LaunchVelocity = (GetActorForwardVector() * LaunchForce) + (GetActorUpVector() * JumpForce);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMyBossGolem::setGroundFrictionZero, 2.5f, true, 0.0f);
	LaunchCharacter(LaunchVelocity, false, false);
	GetCharacterMovement()->GroundFriction = 8.0f;
}

void AMyBossGolem::Launcher_Backward()
{
	float JumpForce = 1.0f;
	FVector LaunchVelocity = ((GetActorForwardVector()*-1) * LaunchForce) + (GetActorUpVector() * JumpForce);
	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMyBossGolem::setGroundFrictionZero, 2.5f, true, 0.0f);
	LaunchCharacter(LaunchVelocity, false, false);
	GetCharacterMovement()->GroundFriction = 8.0f;
}


void AMyBossGolem::setGroundFrictionZero()
{
	GetCharacterMovement()->GroundFriction = 0.0f;
}

GOLEM_ANIM_MONTAGE AMyBossGolem::setRandomAttackMontage(GOLEM_ANIM_MONTAGE Min, GOLEM_ANIM_MONTAGE Max)
{
	int rnd = FMath::RandRange(Min + 1, Max - 1);
	switch (rnd)
	{
	case 1:
		if (!IsBreakLeftArm)
			return NORMAL_ATTACK;
		break;
	case 2:
		if ((!IsBreakLeftArm && !IsBreakRightArm) && RndAtkMtg != HANDCLAP_ATTACK)
			return HANDCLAP_ATTACK;
		else if (IsBreakLeftArm)
			return SWEEP_ATTACK;
		else if (IsBreakRightArm)
			return PUNCH_ATTACK;
		break;
	case 3:
		if (!IsBreakRightArm)
			return SWEEP_ATTACK;
		else
			return PUNCH_ATTACK;
		break;
	case 4:
		if (!IsBreakLeftArm)
			return PUNCH_ATTACK;
		else
			return SWEEP_ATTACK;
		break;
	case 5:
		LaunchForce = 2000.0f;
		IsStomping = true;
		return STOMP_ATTACK;
		break;
	case 6:
		LaunchForce = 1000.0f;
		IsWalkingAtk = true;
		return WALKING_ATTACK;
		break;
	case 7:
		LaunchForce = 1500.0f;
		IsRushing = true;
		return RUSH_CLOSE;
		break;
	case 8:
		return ICE_EXPLO;
		break;
	case 11: // Long Range
		LaunchForce = 1500.0f;
		IsRushing = true;
		return RUSH_ATTACK;
		break;
	case 12:
		if (!IsHalfBreakLeftArm && !IsBreakLeftArm && !IsHalfBreakRightArm && !IsBreakRightArm && RndAtkMtg != THROW_STONE)
			return THROW_STONE;
		break;
	case 13:
		if (!IsHalfBreakRightArm && !IsBreakRightArm && RndAtkMtg != THROW_SPEAR)
			return THROW_SPEAR;
		break;
	case 16: // Down State
		if (!IsBreakLeftArm && !IsHalfBreakLeftArm)
			return DOWN_ATTACK_1;
		else
			return DOWN_ATTACK_2;
		break;
	case 17:
		if (!IsBreakRightArm && !IsHalfBreakRightArm)
			return DOWN_ATTACK_2;
		else
			return DOWN_ATTACK_1;
		break;
	case 18:
		if (!IsBreakLeftArm && !IsHalfBreakLeftArm) {
			return DOWN_SWEEP;
		}
		else {
			return DOWN_ATTACK_1;
		}
		break;
	default:
		if (IsDown)
			return SWEEP_ATTACK;
		else
			return NORMAL_ATTACK;
		break;
		// 이전 오류 안나던 case문
		/*case 1:
			return NORMAL_ATTACK;
		case 2:
			if ((!IsBreakLeftArm || !IsBreakRightArm) && RndAtkMtg != HANDCLAP_ATTACK)
				return HANDCLAP_ATTACK;
			else
		case 3:
			if(IsBreakLeftArm)
				return SWEEP_ATTACK;
			else
				return PUNCH_ATTACK;
		case 4:
			if(IsBreakRightArm)
				return PUNCH_ATTACK;
			else
				return SWEEP_ATTACK;
		case 5:
			return STOMP_ATTACK;
		case 6:
			if (!IsBreakLeftArm && !IsBreakRightArm && RndAtkMtg != WALKING_ATTACK)
			{
				LaunchForce = 1000.0f;
				return WALKING_ATTACK;
			}
			else
				return STOMP_ATTACK;
		case 9:
			if (!IsBreakLeftArm && !IsBreakRightArm && RndAtkMtg != RUSH_ATTACK)
			{
				LaunchForce = 1000.0f;
				return RUSH_ATTACK;
			}
		case 10:
			return THROW_STONE;
		case 11:
			return THROW_SPEAR;
		case 14:
			if (IsBreakLeftArm)
				return DOWN_ATTACK_1;
			else
				return DOWN_ATTACK_2;
		case 15:
			if(IsBreakRightArm)
				return DOWN_ATTACK_2;
			else
				return DOWN_ATTACK_1;*/
	}
	return GOLEM_ANIM_MONTAGE();
}

void AMyBossGolem::ChargeSpearLoopStart()
{
	GolemAnim->JumpToThrowSpearMontageSection(FName("Loop"));
}

void AMyBossGolem::ChargeSpearLoopEnd()
{
	GolemAnim->JumpToThrowSpearMontageSection(FName("End"));
}

void AMyBossGolem::ChargeStompingLoopStart()
{
	GolemAnim->JumpToStompAttackMontageSection(FName("Loop"));
}
void AMyBossGolem::ChargeStompingLoopEnd()
{
	GolemAnim->JumpToStompAttackMontageSection(FName("End"));
}

void AMyBossGolem::ChargeDownSweepLoopStart()
{
	GolemAnim->JumpToDownSweepMontageSection(FName("End"));
}

void AMyBossGolem::ChargeDownSweepLoopEnd()
{
	GolemAnim->JumpToDownSweepMontageSection(FName("End"));
}


void AMyBossGolem::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	ABCHECK(IsAttacking);
	IsAttacking = false;
	IsRushing = false;
	IsStomping = false;
	IsWalkingAtk = false;
	//IsFalling = false;
	OnAttackEnd.Broadcast();
}

// Called when the game starts or when spawned
void AMyBossGolem::BeginPlay()
{
	Super::BeginPlay();

	BoneMap.Emplace("Bip001-R-Hand", 20);
	BoneMap.Emplace("Bip001-R-Forearm", 22);
	BoneMap.Emplace("Bip001-R-UpperArm", 24);
	BoneMap.Emplace("Bip001-R-Foot", 30);
	BoneMap.Emplace("Bip001-R-Calf", 32);
	BoneMap.Emplace("Bip001-R-Thigh", 34);
	BoneMap.Emplace("Bip001-L-Hand", 10);
	BoneMap.Emplace("Bip001-L-Forearm", 14);
	BoneMap.Emplace("Bip001-L-UpperArm", 18);
	BoneMap.Emplace("Bip001-L-Foot", 30);
	BoneMap.Emplace("Bip001-L-Calf", 32);
	BoneMap.Emplace("Bip001-L-Thigh", 34);	
}

void AMyBossGolem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GolemAnim = Cast<UBossGolemAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != GolemAnim);

	GolemAnim->OnMontageEnded.AddDynamic(this, &AMyBossGolem::OnAttackMontageEnded);
	// 여기서 캡슐 컴포넌트 애드 다이내믹 ㄱㄱ
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyBossGolem::OnOverlapBegin);
}

// Called every frame
void AMyBossGolem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*if (!IsDownInit && IsDown)
	{
		GolemAnim->PlayGolemMontage(GOLEM_ANIM_MONTAGE::BROKENLEG_FALLING);
		IsFalling = true;
		IsDownInit = true;
	}*/
	/*if (IsCreatingSpear)
	{
		GetWorldTimerManager().SetTimer(TimerHandle, this, &AMyBossGolem::ChargeSpear, 2.5f, true, 0.0f);
	}*/
}

// Called to bind functionality to input
void AMyBossGolem::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

float AMyBossGolem::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	ABLOG(Warning, TEXT("Actor: %s took Damage: %f"), *GetName(), FinalDamage);
	return FinalDamage;
}

void AMyBossGolem::BreakLegsCpp(AActor * Object)
{
	FOutputDeviceNull ar;
	Object->CallFunctionByNameWithArguments(*FString::Printf(TEXT("BreakLegs "), Object), ar, nullptr, true);
}


void AMyBossGolem::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 여기에서 블프 함수 실행
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		/*FOutputDeviceNull ar;
		this->CallFunctionByNameWithArguments(*FString::Printf(TEXT("BreakLegs "), OtherActor), ar, nullptr, true);*/
		BreakLegsCpp(OtherActor);
	}
}
void AMyBossGolem::SetID(const int& id) {
	this->id = id;
	type = OBJ_GOLEM;
}


//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "MyBossGolem.h"
//#include "BossGolemAnimInstance.h"
//
//#define B_MAP_0 "Bip001-R-Hand"
//#define B_MAP_1 "Bip001-R-Forearm"
//#define B_MAP_2 "Bip001-R-UpperArm"
//#define B_MAP_3 "Bip001-R-Foot"
//#define B_MAP_4 "Bip001-R-Calf"
//#define B_MAP_5 "Bip001-R-Thigh"
//#define B_MAP_6 "Bip001-L-Hand"
//#define B_MAP_7 "Bip001-L-Forearm"
//#define B_MAP_8 "Bip001-L-UpperArm"
//#define B_MAP_9 "Bip001-L-Foot"
//#define B_MAP_10 "Bip001-L-Calf"
//#define B_MAP_11 "Bip001-L-Thigh"
//
//#define R_HAND 0
//#define R_FOREARM 1
//#define R_UPPERARM 2
//#define R_FOOR 3
//#define R_CALF 4
//#define R_THIGHT 5
//#define L_HAND 6
//#define L_FOREARM 7
//#define L_UPPERARM 8
//#define L_FOOR 9
//#define L_CALF 10
//#define L_THIGHT 11
//
//extern Network net;
//
//// Sets default values
//AMyBossGolem::AMyBossGolem()
//{
// 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = true;
//	GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyMonster"));
//	bUseControllerRotationYaw = false;
//	GetCharacterMovement()->bUseControllerDesiredRotation = false;
//	GetCharacterMovement()->bOrientRotationToMovement = true;
//	GetCharacterMovement()->RotationRate = FRotator(0.0f, 480.0f, 0.0f);
//	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
//	AIControllerClass = AMyAIController::StaticClass();
//	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
//	IsAttacking = false;
//	IsRushing = false;
//	IsWalkingAtk = false;
//	IsStomping = false;
//	IsDown = false;
//	IsFalling = false;
//	IsDetectInit = false;
//	IsPlayerInDetRange = false;
//	id = 20000;
//	type = -1;
//}
//
//void AMyBossGolem::Attack_CloseRange()
//{
//	if (IsAttacking) return;
//	if (IsFalling) return;
//	if (!IsDown)
//	{
//		RndAtkMtg = setRandomAttackMontage(CLOSE_RANGE_START, CLOSE_RANGE_END);
//	}
//	else
//	{
//		RndAtkMtg = setRandomAttackMontage(DOWN_START, DOWN_END);
//		//RndAtkMtg = DOWN_SWEEP;
//		/*if (RndDownAtk == 0)
//		{
//			GolemAnim->PlayDownAttack_1_Montage();
//		}
//		else
//		{
//			GolemAnim->PlayDownAttack_2_Montage();
//		}*/
//	}
//	GolemAnim->PlayGolemMontage(RndAtkMtg);
//	//GolemAnim->PlayGolemMontage(THROW_STONE);
//
//
//	IsAttacking = true;
//}
//
//void AMyBossGolem::Attack_LongRange()
//{
//	if (IsAttacking) return;
//	if (IsFalling) return;
//	if (!IsDown)
//	{
//		RndAtkMtg = setRandomAttackMontage(LONG_RANGE_START, LONG_RANGE_END);
//	}
//	//GolemAnim->PlayGolemMontage(THROW_STONE);
//	GolemAnim->PlayGolemMontage(RndAtkMtg);
//	IsAttacking = true;
//}
//
//
//bool AMyBossGolem::getIsAttacking()
//{
//	return IsAttacking;
//}
//
//bool AMyBossGolem::getIsDown()
//{
//	return IsDown;
//}
//
//bool AMyBossGolem::getIsFalling()
//{
//	return IsFalling;
//}
//
//GOLEM_ANIM_MONTAGE AMyBossGolem::getRndAtkMtg()
//{
//	return RndAtkMtg;
//}
//
//void AMyBossGolem::Launcher()
//{
//	float JumpForce = 1.0f;
//	FVector LaunchVelocity = (GetActorForwardVector() * LaunchForce) + (GetActorUpVector() * JumpForce);
//	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMyBossGolem::setGroundFrictionZero, 2.5f, true, 0.0f);
//	LaunchCharacter(LaunchVelocity, false, false);
//	GetCharacterMovement()->GroundFriction = 8.0f;
//}
//
//void AMyBossGolem::Launcher_Backward()
//{
//	float JumpForce = 1.0f;
//	FVector LaunchVelocity = ((GetActorForwardVector()*-1) * LaunchForce) + (GetActorUpVector() * JumpForce);
//	GetWorldTimerManager().SetTimer(TimerHandle, this, &AMyBossGolem::setGroundFrictionZero, 2.5f, true, 0.0f);
//	LaunchCharacter(LaunchVelocity, false, false);
//	GetCharacterMovement()->GroundFriction = 8.0f;
//}
//
//
//void AMyBossGolem::setGroundFrictionZero()
//{
//	GetCharacterMovement()->GroundFriction = 0.0f;
//}
//
//GOLEM_ANIM_MONTAGE AMyBossGolem::setRandomAttackMontage(GOLEM_ANIM_MONTAGE Min, GOLEM_ANIM_MONTAGE Max)
//{
//	//if (net.isHost == false) return GOLEM_ANIM_MONTAGE();
//	int rnd = FMath::RandRange(Min + 1, Max - 1);
//	CS_BOSS_ATTACK pack{ sizeof(CS_BOSS_ATTACK), cs_boss_attack, id };
//	switch (rnd)
//	{
//	case 1:
//		if (!IsBreakLeftArm) {
//			pack.atk_num = NORMAL_ATTACK;
//			net.SendPacket(&pack);
//			return NORMAL_ATTACK;
//		}
//		break;
//	case 2:
//		if ((!IsBreakLeftArm && !IsBreakRightArm) && RndAtkMtg != HANDCLAP_ATTACK) {
//			pack.atk_num = HANDCLAP_ATTACK;
//			net.SendPacket(&pack);
//			return HANDCLAP_ATTACK;
//		}
//		else if (IsBreakLeftArm) {
//			pack.atk_num = SWEEP_ATTACK;
//			net.SendPacket(&pack);
//			return SWEEP_ATTACK;
//		}
//		else if (IsBreakRightArm) {
//			pack.atk_num = PUNCH_ATTACK;
//			net.SendPacket(&pack);
//			return PUNCH_ATTACK;
//		}
//		break;
//	case 3:
//		if (!IsBreakRightArm) {
//			pack.atk_num = SWEEP_ATTACK;
//			net.SendPacket(&pack);
//			return SWEEP_ATTACK;
//		}
//		else {
//			pack.atk_num = PUNCH_ATTACK;
//			net.SendPacket(&pack);
//			return PUNCH_ATTACK;
//		}
//		break;
//	case 4:
//		if (!IsBreakLeftArm) {
//			pack.atk_num = PUNCH_ATTACK;
//			net.SendPacket(&pack);
//			return PUNCH_ATTACK;
//		}
//		else {
//			pack.atk_num = SWEEP_ATTACK;
//			net.SendPacket(&pack);
//			return SWEEP_ATTACK;
//		}
//		break;
//	case 5:
//		pack.atk_num = STOMP_ATTACK;
//		net.SendPacket(&pack);
//		LaunchForce = 2000.0f;
//		IsStomping = true;
//		return STOMP_ATTACK;
//		break;
//	case 6:
//		pack.atk_num = WALKING_ATTACK;
//		net.SendPacket(&pack);
//		LaunchForce = 1000.0f;
//		IsWalkingAtk = true;
//		return WALKING_ATTACK;
//		break;
//	case 7:
//		pack.atk_num = RUSH_ATTACK;
//		net.SendPacket(&pack);
//		LaunchForce = 1000.0f;
//		IsRushing = true;
//		return RUSH_CLOSE;
//		break;
//	case 8:
//		pack.atk_num = ICE_EXPLO;
//		net.SendPacket(&pack);
//		return ICE_EXPLO;
//		break;
//	case 11: // Long Range
//		pack.atk_num = LONG_RUSH_ATTACK;
//		net.SendPacket(&pack);
//		LaunchForce = 1500.0f;
//		IsRushing = true;
//		return RUSH_ATTACK;
//		break;
//	case 12:
//		if (!IsHalfBreakLeftArm && !IsBreakLeftArm && !IsHalfBreakRightArm && !IsBreakRightArm && RndAtkMtg != THROW_STONE) {
//			pack.atk_num = THROW_STONE;
//			net.SendPacket(&pack);
//			return THROW_STONE;
//		}
//		break;
//	case 13:
//		if (!IsHalfBreakRightArm && !IsBreakRightArm && RndAtkMtg != THROW_SPEAR) {
//			pack.atk_num = THROW_SPEAR;
//			net.SendPacket(&pack);
//			return THROW_SPEAR;
//		}
//		break;
//	case 16: // Down State
//		if (!IsBreakLeftArm /*&& !IsHalfBreakLeftArm*/) {
//			pack.atk_num = DOWN_ATTACK_1;
//			net.SendPacket(&pack);
//			return DOWN_ATTACK_1;
//		}
//		else {
//			pack.atk_num = DOWN_ATTACK_2;
//			net.SendPacket(&pack);
//			return DOWN_ATTACK_2;
//		}
//		break;
//	case 17:
//		if (!IsBreakRightArm /*&& !IsHalfBreakRightArm*/) {
//			pack.atk_num = DOWN_ATTACK_2;
//			net.SendPacket(&pack);
//			return DOWN_ATTACK_2;
//		}
//		else {
//			pack.atk_num = DOWN_ATTACK_1;
//			net.SendPacket(&pack);
//			return DOWN_ATTACK_1;
//		}
//		break;
//	case 18:
//		if (!IsBreakLeftArm /*&& !IsHalfBreakLeftArm*/) {
//			pack.atk_num = DOWN_SWEEP;
//			net.SendPacket(&pack);
//			return DOWN_SWEEP;
//		}
//		else {
//			pack.atk_num = DOWN_ATTACK_1;
//			net.SendPacket(&pack);
//			return DOWN_ATTACK_1;
//		}
//		break;
//	default:
//		if (IsDown) {
//			pack.atk_num = DOWN_ATTACK_1;
//			net.SendPacket(&pack);
//			return DOWN_ATTACK_1;
//		}
//		else {
//			pack.atk_num = NORMAL_ATTACK;
//			net.SendPacket(&pack);
//			return NORMAL_ATTACK;
//		}
//		break;
//	// 이전 오류 안나던 case문
//	/*case 1:
//		return NORMAL_ATTACK;
//	case 2:
//		if ((!IsBreakLeftArm || !IsBreakRightArm) && RndAtkMtg != HANDCLAP_ATTACK)
//			return HANDCLAP_ATTACK;
//		else
//	case 3:
//		if(IsBreakLeftArm)
//			return SWEEP_ATTACK;
//		else
//			return PUNCH_ATTACK;
//	case 4:
//		if(IsBreakRightArm)
//			return PUNCH_ATTACK;
//		else
//			return SWEEP_ATTACK;
//	case 5:
//		return STOMP_ATTACK;
//	case 6:
//		if (!IsBreakLeftArm && !IsBreakRightArm && RndAtkMtg != WALKING_ATTACK)
//		{
//			LaunchForce = 1000.0f;
//			return WALKING_ATTACK;
//		}
//		else
//			return STOMP_ATTACK;
//	case 9:
//		if (!IsBreakLeftArm && !IsBreakRightArm && RndAtkMtg != RUSH_ATTACK)
//		{
//			LaunchForce = 1000.0f;
//			return RUSH_ATTACK;
//		}
//	case 10:
//		return THROW_STONE;
//	case 11:
//		return THROW_SPEAR;
//	case 14:
//		if (IsBreakLeftArm)
//			return DOWN_ATTACK_1;
//		else
//			return DOWN_ATTACK_2;
//	case 15:
//		if(IsBreakRightArm)
//			return DOWN_ATTACK_2;
//		else
//			return DOWN_ATTACK_1;*/
//	}
//	return GOLEM_ANIM_MONTAGE();
//}
//
//void AMyBossGolem::ChargeSpearLoopStart()
//{
//	GolemAnim->JumpToThrowSpearMontageSection(FName("Loop"));
//}
//
//void AMyBossGolem::ChargeSpearLoopEnd()
//{
//	GolemAnim->JumpToThrowSpearMontageSection(FName("End"));
//}
//
//void AMyBossGolem::ChargeStompingLoopStart()
//{
//	GolemAnim->JumpToStompAttackMontageSection(FName("Loop"));
//}
//void AMyBossGolem::ChargeStompingLoopEnd()
//{
//	GolemAnim->JumpToStompAttackMontageSection(FName("End"));
//}
//
//void AMyBossGolem::ChargeDownSweepLoopStart()
//{
//	GolemAnim->JumpToDownSweepMontageSection(FName("End"));
//}
//
//void AMyBossGolem::ChargeDownSweepLoopEnd()
//{
//	GolemAnim->JumpToDownSweepMontageSection(FName("End"));
//}
//
//
//
//
//void AMyBossGolem::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
//{
//	ABCHECK(IsAttacking);
//	IsAttacking = false;
//	IsRushing = false;
//	IsStomping = false;
//	IsWalkingAtk = false;
//	//IsFalling = false;
//	OnAttackEnd.Broadcast();
//}
//
//// Called when the game starts or when spawned
//void AMyBossGolem::BeginPlay()
//{
//	Super::BeginPlay();
//	BoneMap.Emplace("Bip001-R-Hand", 20);
//	BoneMap.Emplace("Bip001-R-Forearm", 22);
//	BoneMap.Emplace("Bip001-R-UpperArm", 24);
//	BoneMap.Emplace("Bip001-R-Foot", 30);
//	BoneMap.Emplace("Bip001-R-Calf", 32);
//	BoneMap.Emplace("Bip001-R-Thigh", 34);
//	BoneMap.Emplace("Bip001-L-Hand", 10);
//	BoneMap.Emplace("Bip001-L-Forearm", 14);
//	BoneMap.Emplace("Bip001-L-UpperArm", 18);
//	BoneMap.Emplace("Bip001-L-Foot", 30);
//	BoneMap.Emplace("Bip001-L-Calf", 32);
//	BoneMap.Emplace("Bip001-L-Thigh", 34);
//
//	pos = GetActorLocation();
//	rotate = GetActorRotation();
//
//	sendTime = high_resolution_clock::now();
//}
//
//void AMyBossGolem::PostInitializeComponents()
//{
//	Super::PostInitializeComponents();
//	GolemAnim = Cast<UBossGolemAnimInstance>(GetMesh()->GetAnimInstance());
//	ABCHECK(nullptr != GolemAnim);
//
//	GolemAnim->OnMontageEnded.AddDynamic(this, &AMyBossGolem::OnAttackMontageEnded);
//	// 여기서 캡슐 컴포넌트 애드 다이내믹 ㄱㄱ
//	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyBossGolem::OnOverlapBegin);
//}
//
//// Called every frame
//void AMyBossGolem::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//	/*if (!IsDownInit && IsDown)
//	{
//		GolemAnim->PlayGolemMontage(GOLEM_ANIM_MONTAGE::BROKENLEG_FALLING);
//		IsFalling = true;
//		IsDownInit = true;
//	}*/
//	/*if (IsCreatingSpear)
//	{
//		GetWorldTimerManager().SetTimer(TimerHandle, this, &AMyBossGolem::ChargeSpear, 2.5f, true, 0.0f);
//	}*/
//	//if (net.isHost) {
//	//	pos = GetActorLocation();
//	//	rotate = GetActorRotation();
//	//	if (sendTime + 50ms < high_resolution_clock::now()) {
//	//		CS_BOSS_MOVE pack{ sizeof(CS_NPC_MOVE), cs_boss_move };
//	//		pack.destination = { pos.X, pos.Y, pos.Z };
//	//		pack.rotation = { rotate.Pitch, rotate.Yaw, rotate.Roll };
//	//		net.SendPacket(&pack);
//	//		sendTime = high_resolution_clock::now();
//
//	//		auto bonePack = MakeBonePacket();
//	//		net.SendPacket(&bonePack);
//	//	}
//	//	return;
//	//}
//	//else {
//	//	if (sendTime + 100ms < high_resolution_clock::now()) {
//	//		auto bonePack = MakeBonePacket();
//	//		net.SendPacket(&bonePack);
//	//		sendTime = high_resolution_clock::now();
//	//	}
//	//}
//
//	//if (net.objEventQue[id].empty()) return;
//	//auto ev = net.objEventQue[id].front();
//	//net.objEventQue[id].pop();
//
//	//switch (ev.type) {
//	//case sc_boss_attack:
//	//	AttackPacketProcess(ev.exp);
//	//	break;
//	//case sc_update_obj:
//	//	pos = { ev.pos.x, ev.pos.y, ev.pos.z };
//	//	rotate = { ev.rotation.x, ev.rotation.y, ev.rotation.z };
//	//	SetActorLocation(pos);
//	//	// SetActorLocationAndRotation(pos, rotate, false, 0, ETeleportType::TeleportPhysics);
//	//	break;
//	//case sc_bone_break:
//	//	BoneMap[GetPartString(ev.o_type)] = 0;
//	//	break;
//	//case sc_bone_update:
//	//	BoneMap[GetPartString(ev.o_type)] = ev.hp;
//	//	break;
//	//default:
//	//	break;
//	//}
//}
//
//// Called to bind functionality to input
//void AMyBossGolem::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//}
//
//float AMyBossGolem::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
//{
//	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
//	ABLOG(Warning, TEXT("Actor: %s took Damage: %f"), *GetName(), FinalDamage);
//	return FinalDamage;
//}
//
//void AMyBossGolem::BreakLegsCpp(AActor * Object)
//{
//	FOutputDeviceNull ar;
//	Object->CallFunctionByNameWithArguments(*FString::Printf(TEXT("BreakLegs "),Object), ar, nullptr, true);
//}
//
//
//void AMyBossGolem::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
//{
//	// 여기에서 블프 함수 실행
//	if (OtherActor && (OtherActor != this) && OtherComp)
//	{
//		/*FOutputDeviceNull ar;
//		this->CallFunctionByNameWithArguments(*FString::Printf(TEXT("BreakLegs "), OtherActor), ar, nullptr, true);*/
//		BreakLegsCpp(OtherActor);
//	}
//}
//
//void AMyBossGolem::AttackPacketProcess(const int& atk_num) {
//	switch (atk_num) {
//	case STOMP_ATTACK:
//		LaunchForce = 2000.f;
//		IsStomping = true;
//		break;
//	case WALKING_ATTACK:
//		LaunchForce = 1000.f;
//		IsWalkingAtk = true;
//		break;
//	case RUSH_ATTACK:
//		LaunchForce = 1000.f;
//		IsRushing = true;
//		break;
//	case LONG_RUSH_ATTACK:
//		LaunchForce = 1500.f;
//		IsRushing = true;
//		break;
//	}
//
//	RndAtkMtg = (GOLEM_ANIM_MONTAGE)atk_num;
//	if (atk_num == LONG_RUSH_ATTACK)
//		RndAtkMtg = RUSH_ATTACK;
//
//	GolemAnim->PlayGolemMontage(RndAtkMtg);
//	IsAttacking = true;
//
//}
//
//CS_BOSS_BONE AMyBossGolem::MakeBonePacket() {
//	CS_BOSS_BONE pack;
//	pack.size = sizeof(CS_BOSS_BONE);
//	pack.oid = id;
//	pack.type = cs_boss_bone;
//	pack.boneMap[R_HAND] = BoneMap[B_MAP_0];
//	pack.boneMap[R_FOREARM] = BoneMap[B_MAP_1];
//	pack.boneMap[R_UPPERARM] = BoneMap[B_MAP_2];
//	pack.boneMap[R_FOOR] = BoneMap[B_MAP_3];
//	pack.boneMap[R_CALF] = BoneMap[B_MAP_4];
//	pack.boneMap[R_THIGHT] = BoneMap[B_MAP_5];
//	pack.boneMap[L_HAND] = BoneMap[B_MAP_6];
//	pack.boneMap[L_FOREARM] = BoneMap[B_MAP_7];
//	pack.boneMap[L_UPPERARM] = BoneMap[B_MAP_8];
//	pack.boneMap[L_FOOR] = BoneMap[B_MAP_9];
//	pack.boneMap[L_CALF] = BoneMap[B_MAP_10];
//	pack.boneMap[L_THIGHT] = BoneMap[B_MAP_11];
//	return pack;
//}
//
//char* AMyBossGolem::GetPartString(int pid) {
//	switch (pid) {
//	case R_HAND:
//		return B_MAP_0;
//		break;
//	case R_FOREARM:
//		return B_MAP_1;
//		break;
//	case R_UPPERARM:
//		return B_MAP_2;
//		break;
//	case R_FOOR:
//		return B_MAP_3;
//		break;
//	case R_CALF:
//		return B_MAP_4;
//		break;
//	case R_THIGHT:
//		return B_MAP_5;
//		break;
//	case L_HAND:
//		return B_MAP_6;
//		break;
//	case L_FOREARM:
//		return B_MAP_7;
//		break;
//	case L_UPPERARM:
//		return B_MAP_8;
//		break;
//	case L_FOOR:
//		return B_MAP_9;
//		break;
//	case L_CALF:
//		return B_MAP_10;
//		break;
//	case L_THIGHT:
//		return B_MAP_11;
//		break;
//	default:
//		break;
//	}
//	return "";
//}
//
