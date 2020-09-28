// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCharacter.h"
#include "MyAnimInstance.h"
#include "DrawDebugHelpers.h"
#include "Network.h"
extern Network net;

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SPRINGARM"));
	//Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CAMERA"));

	//SpringArm->SetupAttachment(GetCapsuleComponent());
	//Camera->SetupAttachment(SpringArm);

	//GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -88.0f), FRotator(0.0f, -90.0f, 0.0f));
	//SpringArm->TargetArmLength = 500.0f;
	//SpringArm->SetRelativeRotation(FRotator(-15.0f, 0.0f, 0.0f));

	//// static을 주석 처리 해놨었는데 왤까 -> 자꾸 71% 무한로딩 걸림
	///*static*/ ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_ADAM(TEXT("/Game/Game/Mesh/Player_Character/Adam_Adventurer/Meshes/Character/SK_AdamAdventurer"));
	/*static ConstructorHelpers::FObjectFinder<UClass> SWORD_BP(TEXT("Blueprint'/Game/Game/BluePrints/player/BP_Sword.BP_Sword'"));
	static ConstructorHelpers::FObjectFinder<UClass> SHIELD_BP(TEXT("Blueprint'/Game/Game/BluePrints/player/BP_Hammer.BP_Hammer'"));*/


	HandSocket1_cpp = CreateDefaultSubobject<USceneComponent>(TEXT("HANDSOCKET1_CPP"));
	HandSocket1_cpp->SetupAttachment(GetMesh());

	swordStrap_cpp = CreateDefaultSubobject<USceneComponent>(TEXT("SWORDSTRAP_CPP"));
	swordStrap_cpp->SetupAttachment(GetMesh());
	
	BPSword_cpp = CreateDefaultSubobject<UChildActorComponent>(TEXT("BPSWORD_CPP"));
	BPSword_cpp->SetupAttachment(swordStrap_cpp);
	BPShield_cpp = CreateDefaultSubobject<UChildActorComponent>(TEXT("BPSHEILD_CPP"));
	BPShield_cpp->SetupAttachment(swordStrap_cpp);

	HandSocket2_cpp = CreateDefaultSubobject<USceneComponent>(TEXT("HANDSOCKET2_CPP"));
	HandSocket2_cpp->SetupAttachment(GetMesh());

	HandSocket3_cpp = CreateDefaultSubobject<USceneComponent>(TEXT("HANDSOCKET3_CPP"));
	HandSocket3_cpp->SetupAttachment(GetMesh());
	
	HammerStrap_cpp = CreateDefaultSubobject<USceneComponent>(TEXT("HAMMERSTRAP_CPP"));
	HammerStrap_cpp->SetupAttachment(GetMesh());
	BPSword_cpp = CreateDefaultSubobject<UChildActorComponent>(TEXT("BPHAMMER_CPP"));
	BPSword_cpp->SetupAttachment(HammerStrap_cpp);

	/*BPSword_cpp->SetChildActorClass(SWORD_BP.Object);
	BPShield_cpp->SetChildActorClass(SHIELD_BP.Object);*/


	//if (SK_ADAM.Succeeded())
	//{
	//	GetMesh()->SetSkeletalMesh(SK_ADAM.Object);
	//}

	//GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	//// static을 주석 처리 해놨었는데 왤까 -> 자꾸 71% 무한로딩 걸림
	///*static*/ ConstructorHelpers::FClassFinder<UAnimInstance> ADAM_ANIM(TEXT("/Game/Game/BluePrints/Animation/character/AdamAdventurer_AnimBP_c++"));

	//if (ADAM_ANIM.Succeeded())
	//{
	//	GetMesh()->SetAnimInstanceClass(ADAM_ANIM.Class);
	//}
	//GetCapsuleComponent()->SetCollisionProfileName(TEXT("MyCharacter"));

	//MaxWalkSpeed = 400.0f;
	//IsAttacking = false;
	//MaxCombo = 5;
	//AttackRange = 200.0f;
	//AttackRadius = 50.0f;

	//AttackEndComboState();
	CanMove_cpp = true;

	id = -1;
	velocity = { 0,0,0 };
	speed = 0.f;
	isMoving = false;
}
//5/31: 290p 세팅까지 마친상태

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	SetActorLocation(FVector(10227.3125, 76509.90625, -437.344971));
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	position = GetActorLocation();
	net.my_pos = { position.X, position.Y, position.Z };


	if (net.GetStatus() != p_login) return;
	if (id == net.GetMyID()) {
		// if (net.isMoving == false) return;
		position = GetActorLocation();
		rotation = GetActorRotation();
		auto vel = GetVelocity();
		CS_MOVE pack;
		pack.size = sizeof(CS_MOVE);
		pack.type = move_packet;

		pack.destination = { position.X, position.Y, position.Z };
		pack.rotation = { rotation.Pitch, rotation.Yaw, rotation.Roll };
		pack.velocity = { vel.X, vel.Y, vel.Z };

		net.SendPacket(&pack);
	}
	net.eventLock.lock();
	if (net.eventQue.empty()) {
		net.eventLock.unlock();
		return;
	}
	auto ev = net.eventQue.front();
	net.eventLock.unlock();
	if (ev.type == sc_login_ok && id == -1) {
		id = net.GetMyID();
		net.PopEvent();
		return;
	}
	if (ev.oid == net.GetMyID()) {
		// 자신의 처리는 이미 끝났으니 일단 스킵 
		net.PopEvent();
		return;
	}
	if (ev.oid >= 10) return;
	if (ev.oid != id && id != -1) return;
	AddMovementInput(velocity, speed);
	isMoving = false;

	switch (ev.type) {
	case sc_login_ok:
		if (id != -1) return;
		id = net.GetMyID();
		net.PopEvent();
		break;
	case sc_update_obj: {
		position = { ev.pos.x, ev.pos.y, ev.pos.z };
		rotation = { ev.rotation.x, ev.rotation.y, ev.rotation.z };
		velocity = { ev.velocity.x,ev.velocity.y ,ev.velocity.z };
		// UE_LOG(LogTemp, Log, TEXT("%d %d %d"), ev.velocity.x, ev.velocity.y, ev.velocity.z);
		// SetActorRotation(rotation);
		// SetActorLocation(position);
		// auto currentPos = GetActorLocation();
		// if ((int)currentPos.X == (int)position.X) {
		// 	UE_LOG(LogTemp, Log, TEXT("C Movesaddsa"));
		// 	break;
		// }
		isMoving = true;
		speed = 600.f;
		// speed = 0.f;
		// GetVelocity();
		// UE_LOG(LogTemp, Log, TEXT("C Move"));
		// AddMovementInput(position);
		// AddMovementInput(FRotationMatrix(rotation).GetUnitAxis(EAxis::Y), 200.f);
		// AddMovementInput(GetVelocity(), 200.f);

		// SetActorLocationAndRotation(position, rotation, false, 0, ETeleportType::None);

		net.PopEvent();
	}break;
	case sc_attack: {
		if (net.wpnType == wpn_none) break;
		UMyAnimInstance* myAnimInst = Cast<UMyAnimInstance>(animInstance);
		if (myAnimInst != nullptr) {
			if (ev.mp == 0)
				myAnimInst->SwordSlashCombo1();
			else if (ev.mp == 1)
				myAnimInst->SwordSlashCombo2();
			else myAnimInst->SwordSlashCombo3();
		}
		net.PopEvent();
	}break;
	case sc_weapon_on: {	// 해머는 나중에 분리 지금은 귀찮아
		BPSword_cpp->AttachTo(HandSocket1_cpp);
		BPShield_cpp->AttachTo(HandSocket2_cpp);
		UMyAnimInstance* myAnimInst = Cast<UMyAnimInstance>(animInstance);
		if (myAnimInst != nullptr) myAnimInst->OutSword();
		net.PopEvent();
	}break;
	case sc_weapon_off: {
		HandSocket1_cpp->DetachFromParent();
		HandSocket2_cpp->DetachFromParent();
		UMyAnimInstance* myAnimInst = Cast<UMyAnimInstance>(animInstance);
		if (myAnimInst != nullptr) myAnimInst->InSword();
		net.PopEvent();
	}break;
	case sc_sword_on: {
		BPSword_cpp->AttachTo(HandSocket1_cpp);
		BPShield_cpp->AttachTo(HandSocket2_cpp);
		UMyAnimInstance* myAnimInst = Cast<UMyAnimInstance>(animInstance);
		if (myAnimInst != nullptr) myAnimInst->OutSword();
		net.PopEvent();
	}break;
	case sc_sword_off: {
		HandSocket1_cpp->DetachFromParent();
		HandSocket2_cpp->DetachFromParent();
		UMyAnimInstance* myAnimInst = Cast<UMyAnimInstance>(animInstance);
		if (myAnimInst != nullptr) myAnimInst->InSword();
		net.PopEvent();
	}break;
	case sc_hammer_on: {
		BPHammer_cpp->AttachTo(HandSocket1_cpp);
		UMyAnimInstance* myAnimInst = Cast<UMyAnimInstance>(animInstance);
		if (myAnimInst != nullptr) myAnimInst->OutHammer();
		net.PopEvent();
	}break;
	case sc_hammer_off: {
		HandSocket3_cpp->DetachFromParent();
		UMyAnimInstance* myAnimInst = Cast<UMyAnimInstance>(animInstance);
		if (myAnimInst != nullptr) myAnimInst->InHammer();
		net.PopEvent();
	}break;
	case sc_guard: {
		UMyAnimInstance* myAnimInst = Cast<UMyAnimInstance>(animInstance);
		if (myAnimInst != nullptr) myAnimInst->SwordShieldIdle();
		net.PopEvent();
	}break;
	case sc_berserk: {
		UMyAnimInstance* myAnimInst = Cast<UMyAnimInstance>(animInstance);
		if (myAnimInst != nullptr) myAnimInst->Berserker();
		bersuckermode_cpp = true;
		net.PopEvent();
	}break;
	case sc_fireball:
		net.PopEvent();
		break;
	case sc_evade: {
		UMyAnimInstance* myAnimInst = Cast<UMyAnimInstance>(animInstance);
		if (myAnimInst != nullptr) myAnimInst->Evade();
		net.PopEvent();
	}break;
	case sc_jump: {
		UMyAnimInstance* myAnimInst = Cast<UMyAnimInstance>(animInstance);
		if (myAnimInst != nullptr) myAnimInst->Jump();
		net.PopEvent();
	}break;
	case sc_level_up:
		net.PopEvent();
		break;
	case sc_dead: {
		UMyAnimInstance* myAnimInst = Cast<UMyAnimInstance>(animInstance);
		if (myAnimInst != nullptr) myAnimInst->Pickup();
		net.PopEvent();
	}break;
	case sc_damaged: {
		UE_LOG(LogTemp, Log, TEXT("Player Damaged!"));
		UMyAnimInstance* myAnimInst = Cast<UMyAnimInstance>(animInstance);
		if (myAnimInst != nullptr) myAnimInst->SwordShieldImpact1();
		hp = ev.hp;
		net.PopEvent();
	}break;
	case sc_get_exp:
		net.PopEvent();
		break;
	case sc_block:
		net.PopEvent();
		break;
	case sc_move_stop:
		speed = 0.f;
		velocity = { 0,0,0 };
		position = { ev.pos.x, ev.pos.y, ev.pos.z };
		SetActorLocation(position);
		net.PopEvent();
		break;
	case sc_set_npc_target: {
		if (ev.hp == 10000) {

		}
		net.PopEvent();
	}break;
	default:
		net.PopEvent();
		break;
	}
}

void AMyCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	animInstance = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	/*MyAnim = Cast<UMyAnimInstance>(GetMesh()->GetAnimInstance());
	ABCHECK(nullptr != MyAnim);
	MyAnim->OnMontageEnded.AddDynamic(this, &AMyCharacter::OnAttackMontageEnded);
	MyAnim->OnNextAttackCheck.AddLambda([this]()->void {
		ABLOG(Warning, TEXT("OnNextAttackCheck"));
		CanNextCombo = false;

		if (IsComboInputOn)
		{
			AttackStartComboState();
			MyAnim->JumpToAttackMontageSection(CurrentCombo);
		}
	});
	MyAnim->OnAttackHitCheck.AddUObject(this, &AMyCharacter::AttackCheck);*/
}

void AMyCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

//float AMyCharacter::TakeDamage(float DamageAmount, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser)
//{
//	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
//	ABLOG(Warning, TEXT("Actor: %s took Damage: %f"), *GetName(), FinalDamage);
//	return FinalDamage;
//}
//
//// Called to bind functionality to input
//void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//	PlayerInputComponent->BindAction(TEXT("Attack"), EInputEvent::IE_Pressed, this, &AMyCharacter::Attack);
//	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AMyCharacter::MoveForward);
//	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AMyCharacter::MoveRight);
//	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AMyCharacter::Turn);
//	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AMyCharacter::LookUp);
//
//}
//
//
//void AMyCharacter::MoveForward(float NewAxisValue)
//{
//	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X), NewAxisValue);
//}
//
//void AMyCharacter::MoveRight(float NewAxisValue)
//{
//	AddMovementInput(FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y), NewAxisValue);
//}
//
//void AMyCharacter::LookUp(float NewAxisValue)
//{
//	AddControllerPitchInput(NewAxisValue);
//}
//
//void AMyCharacter::Turn(float NewAxisValue)
//{
//	AddControllerYawInput(NewAxisValue);
//}
//
//void AMyCharacter::Attack()
//{
//	//ABLOG(Warning, TEXT("Attack()"));
//
//	if (IsAttacking)
//	{
//		ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 1, MaxCombo));
//		if (CanNextCombo)
//		{
//			IsComboInputOn = true;
//			FOutputDeviceNull pAR;
//			float Duration = 2.0f;
//			this->CallFunctionByNameWithArguments(*FString::Printf(TEXT("ActiveSwordTrail %f"), Duration), pAR, nullptr, true);
//
//		}
//	}
//	else
//	{
//		ABCHECK(CurrentCombo == 0);
//		FOutputDeviceNull pAR;
//		float Duration = 2.0f;
//		this->CallFunctionByNameWithArguments(*FString::Printf(TEXT("ActiveSwordTrail %f"), Duration), pAR, nullptr, true);
//		AttackStartComboState();
//		MyAnim->PlayAttackMontage();
//		MyAnim->JumpToAttackMontageSection(CurrentCombo);
//		IsAttacking = true;
//	}
//
//	
//}
//void AMyCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
//{
//	ABCHECK(IsAttacking);
//	ABCHECK(CurrentCombo > 0);
//	IsAttacking = false;
//	AttackEndComboState();
//}
//
//void AMyCharacter::AttackStartComboState()
//{
//	CanNextCombo = true;
//	IsComboInputOn = false;
//	ABCHECK(FMath::IsWithinInclusive<int32>(CurrentCombo, 0, MaxCombo - 1));
//	CurrentCombo = FMath::Clamp<int32>(CurrentCombo + 1, 1, MaxCombo);
//}
//
//void AMyCharacter::AttackEndComboState()
//{
//	IsComboInputOn = false;
//	CanNextCombo = false;
//	CurrentCombo = 0;
//}
//
//void AMyCharacter::AttackCheck()
//{
//	FHitResult HitResult;
//	FCollisionQueryParams Params(NAME_None, false, this);
//	bool bResult = GetWorld()->SweepSingleByChannel(
//		HitResult,
//		GetActorLocation(),
//		GetActorLocation() + GetActorForwardVector()*200.0f,
//		FQuat::Identity,
//		ECollisionChannel::ECC_GameTraceChannel2,
//		FCollisionShape::MakeSphere(50.0f),
//		Params);
//#if ENABLE_DRAW_DEBUG
//	FVector TraceVec = GetActorForwardVector()	*AttackRange;
//	FVector Center = GetActorLocation() + TraceVec * 0.5f;
//	float HalfHeight = AttackRange * 0.5f + AttackRadius;
//	FQuat CapsuleRot = FRotationMatrix::MakeFromZ(TraceVec).ToQuat();
//	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
//	float DebugLifeTime = 5.0f;
//
//	DrawDebugCapsule(GetWorld(),
//		Center,
//		HalfHeight,
//		AttackRadius,
//		CapsuleRot,
//		DrawColor,
//		false,
//		DebugLifeTime
//	);
//#endif
//	if (bResult)
//	{
//		if (HitResult.Actor.IsValid())
//		{
//			ABLOG(Warning, TEXT("Hit Actor Name : %s"), *HitResult.Actor->GetName());
//			FDamageEvent DamageEvent;
//			HitResult.Actor->TakeDamage(10.0f, DamageEvent, GetController(), this); // 여기서 첫번째 인자에 어택 데미지 입력
//		}
//	}
//}

void AMyCharacter::SetID(const int& id) {
	SpawnDefaultController();
	this->id = id;
}

void AMyCharacter::SetPosition(float x, float y, float z) {
	SetActorLocation(FVector(x, y, z));
}