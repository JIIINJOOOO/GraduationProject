// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Last_Hero.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "MyAnimInstance.h"
#include "Network.h"
#include "MyCharacter.generated.h"

UENUM(BlueprintType)
enum class EMovementStateCPP : uint8
{
	MS_WALKING UMETA(DisplayName = "Walking"),
	MS_ONWALL UMETA(DisplayName = "OnWall"),
	MS_JUMPING UMETA(DisplayName = "Jumping"),
	MS_FALING UMETA(DisplayName = "Faling"),
	MS_HANGING UMETA(DisplayName = "Hanging")
};

UCLASS()
class LAST_HERO_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()
private:
	int id;
	FVector position;
	FRotator rotation;
	int hp;
	int mp;
	float speed;
	FVector velocity;
	UMyAnimInstance* animInstance;
	bool isMoving;
	FVector netPos;
	Weapon_Type wpnType;
	bool other;
public:
	// Sets default values for this character's properties
	AMyCharacter();
	//float Dist_Boss;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;
	//void SetDist_Boss(float dist);
	//virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Called to bind functionality to input
	//virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//UPROPERTY(BlueprintReadWrite,VisibleAnywhere, Category = Camera)
	//	USpringArmComponent* SpringArm;

	//UPROPERTY(BlueprintReadWrite,VisibleAnywhere, Category = Camera)
	//	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		bool Running_cpp;								  
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		bool Sprinting_cpp;								   
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		bool CanMove_cpp;								   
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		bool Crouching_cpp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		bool IsCasting_cpp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		bool CanCounter_cpp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		bool underAttack_cpp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		bool bersuckermode_cpp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		bool HammerEquipped_cpp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		bool SwordEquipped_cpp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		EMovementStateCPP MovementState_cpp;
	
	// attack count
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		int AtkCount_cpp;

	// sockets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		USceneComponent* HandSocket1_cpp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		USceneComponent* swordStrap_cpp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		USceneComponent* HandSocket2_cpp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		USceneComponent* HandSocket3_cpp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		USceneComponent* HammerStrap_cpp;

	// child actors : weapons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		UChildActorComponent* BPSword_cpp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		UChildActorComponent* BPShield_cpp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		UChildActorComponent* BPHammer_cpp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PlayerInfo)
		float ServerPlayerHP;

	
//private:
//	void MoveForward(float NewAxisValue);
//	void MoveRight(float NewAxisValue);
//	void LookUp(float NewAxisValue);
//	void Turn(float NewAxisValue);
//	void Attack();
//	UFUNCTION()
//		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
//	void AttackStartComboState();
//	void AttackEndComboState();
//	void AttackCheck();


//private:
	/*UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool CanNextCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsComboInputOn;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 MaxCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRange;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		float AttackRadius;*/

		
	/*UPROPERTY()
		class UMyAnimInstance* MyAnim;*/
public:
	void SetID(const int& id);
	void SetPosition(float x, float y, float z);
};
