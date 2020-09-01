// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Last_Hero.h"
#include "GameFramework/Actor.h"
#include "Animation/AnimMontage.h"
#include "Weapon_Sword.generated.h"

UCLASS()
class LAST_HERO_API AWeapon_Sword : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon_Sword();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		TArray<UAnimMontage*> Sword_regularattacks_cpp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		TArray<UAnimMontage*> Sword_comboattacks_cpp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		TArray<UAnimMontage*> Hammer_regularattacks_cpp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		TArray<UAnimMontage*> Hammer_comboattacks_cpp;
	
	UPROPERTY()
		class UMyAnimInstance* PlayerAnim;

};
