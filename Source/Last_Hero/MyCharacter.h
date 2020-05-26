// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Last_Hero.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "MyCharacter.generated.h"

UCLASS()
class LAST_HERO_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;
	virtual void PossessedBy(AController* NewController) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere, Category = Camera)
		USpringArmComponent* SpringArm;

	UPROPERTY(BlueprintReadWrite,VisibleAnywhere, Category = Camera)
		UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		bool bIsCasting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		bool bIsLooting;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Utilities)
		float MaxWalkSpeed;




private:
	void MoveForward(float NewAxisValue);
	void MoveRight(float NewAxisValue);
	void LookUp(float NewAxisValue);
	void Turn(float NewAxisValue);
	void Attack();
	UFUNCTION()
		void OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted);
	void AttackStartComboState();
	void AttackEndComboState();


private:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsAttacking;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool CanNextCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		bool IsComboInputOn;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 CurrentCombo;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Attack, Meta = (AllowPrivateAccess = true))
		int32 MaxCombo;
	UPROPERTY()
		class UMyAnimInstance* MyAnim;
};
