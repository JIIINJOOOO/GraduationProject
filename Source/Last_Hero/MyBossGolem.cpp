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
	IsDown = false;
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
		return NORMAL_ATTACK;
	case 2:
		if ((!IsBreakLeftArm || !IsBreakRightArm) && RndAtkMtg != HANDCLAP_ATTACK)
			return HANDCLAP_ATTACK;
		else
	case 3:
		if (IsBreakLeftArm)
			return SWEEP_ATTACK;
		else
			return PUNCH_ATTACK;
	case 4:
		if (IsBreakRightArm)
			return PUNCH_ATTACK;
		else
			return SWEEP_ATTACK;
	case 5:
		if (RndAtkMtg != STOMP_ATTACK)
		{
			LaunchForce = 1000.0f;
			return STOMP_ATTACK;
		}
		else
	case 6:
		if (RndAtkMtg != WALKING_ATTACK)
		{
			LaunchForce = 1000.0f;
			return WALKING_ATTACK;
		}
		else 
	case 7: 
		if (RndAtkMtg != RUSH_CLOSE)
		{
			LaunchForce = 1000.0f;
			return RUSH_CLOSE;
		}
		else
			return STOMP_ATTACK;
	case 10: // Long Range
		if (RndAtkMtg != RUSH_ATTACK)
		{
			LaunchForce = 1000.0f;
			return RUSH_ATTACK;
		}
		else
	case 11:
		if (!IsHalfBreakLeftArm && !IsBreakLeftArm && !IsHalfBreakRightArm && !IsBreakRightArm && RndAtkMtg != THROW_STONE)
			return THROW_STONE;
	case 12:
		if (!IsHalfBreakRightArm && !IsBreakRightArm && RndAtkMtg != THROW_SPEAR)
			return THROW_SPEAR;
	case 15: // Down State
		if (!IsHalfBreakLeftArm && !IsBreakLeftArm)
			return DOWN_ATTACK_1;
		else
			return DOWN_ATTACK_2;
	case 16:
		if (!IsHalfBreakRightArm && !IsBreakRightArm)
			return DOWN_ATTACK_2;
		else
			return DOWN_ATTACK_1;
	// ���� ���� �ȳ��� case��
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




void AMyBossGolem::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	ABCHECK(IsAttacking);
	IsAttacking = false;
	IsFalling = false;
	OnAttackEnd.Broadcast();
}

// Called when the game starts or when spawned
void AMyBossGolem::BeginPlay()
{
	Super::BeginPlay();
	BoneMap.Emplace("Bip001-R-Hand", 3);
	BoneMap.Emplace("Bip001-R-Forearm", 5);
	BoneMap.Emplace("Bip001-R-UpperArm", 7);
	BoneMap.Emplace("Bip001-R-Foot", 7);
	BoneMap.Emplace("Bip001-R-Calf", 9);
	BoneMap.Emplace("Bip001-R-Thigh", 11);
	BoneMap.Emplace("Bip001-L-Hand", 3);
	BoneMap.Emplace("Bip001-L-Forearm", 5);
	BoneMap.Emplace("Bip001-L-UpperArm", 7);
	BoneMap.Emplace("Bip001-L-Foot", 7);
	BoneMap.Emplace("Bip001-L-Calf", 9);
	BoneMap.Emplace("Bip001-L-Thigh", 11);
}

void AMyBossGolem::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GolemAnim = Cast<UBossGolemAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != GolemAnim);

	GolemAnim->OnMontageEnded.AddDynamic(this, &AMyBossGolem::OnAttackMontageEnded);
	// ���⼭ ĸ�� ������Ʈ �ֵ� ���̳��� ����
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyBossGolem::OnOverlapBegin);
}

// Called every frame
void AMyBossGolem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsDownInit && IsDown)
	{
		GolemAnim->PlayGolemMontage(GOLEM_ANIM_MONTAGE::BROKENLEG_FALLING);
		IsFalling = true;
		IsDownInit = true;
	}
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
	Object->CallFunctionByNameWithArguments(*FString::Printf(TEXT("BreakLegs "),Object), ar, nullptr, true);
}


void AMyBossGolem::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ���⿡�� ���� �Լ� ����
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		/*FOutputDeviceNull ar;
		this->CallFunctionByNameWithArguments(*FString::Printf(TEXT("BreakLegs "), OtherActor), ar, nullptr, true);*/
		BreakLegsCpp(OtherActor);
	}
}
