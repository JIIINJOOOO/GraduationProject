// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBossGolem.h"
#include "MyAIController.h"
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
}

void AMyBossGolem::Attack()
{
	if (IsAttacking) return;
	if (!IsDown)
	{
		GolemAnim->PlayNormalAttackMontage();
	}
	else
	{
		RndDownAtk = FMath::RandRange(0, 1);
		if (RndDownAtk == 0)
		{
			GolemAnim->PlayDownAttack_1_Montage();
		}
		else
		{
			GolemAnim->PlayDownAttack_2_Montage();
		}
	}
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

int AMyBossGolem::getRndDownAtk()
{
	return RndDownAtk;
}


void AMyBossGolem::OnAttackMontageEnded(UAnimMontage * Montage, bool bInterrupted)
{
	ABCHECK(IsAttacking);
	IsAttacking = false;
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
	// 여기서 캡슐 컴포넌트 애드 다이내믹 ㄱㄱ
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AMyBossGolem::OnOverlapBegin);
	
}

// Called every frame
void AMyBossGolem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
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
	// 여기에서 블프 함수 실행
	if (OtherActor && (OtherActor != this) && OtherComp)
	{
		/*FOutputDeviceNull ar;
		this->CallFunctionByNameWithArguments(*FString::Printf(TEXT("BreakLegs "), OtherActor), ar, nullptr, true);*/
		BreakLegsCpp(OtherActor);
	}
}
