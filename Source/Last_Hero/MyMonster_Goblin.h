// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Last_Hero.h"
#include "GameFramework/Character.h"
#include "UObject/ConstructorHelpers.h"
#include "GoblinAnimInstance.h"
#include "MyMonster_Goblin.generated.h"

UCLASS()
class LAST_HERO_API AMyMonster_Goblin : public ACharacter
{
	GENERATED_BODY()

private:
	int id;
	int hp;
	UGoblinAnimInstance* animInstance;
	FVector velocity;
	float speed;
public:
	// Sets default values for this character's properties
	AMyMonster_Goblin();
	virtual void PostInitializeComponents() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		FVector MonPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterInfo)
		float ServerMonHP;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		bool IsAttacking_cpp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		bool IsBlocking_cpp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		bool IsDodging_cpp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		int RandAttack;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	UFUNCTION(BlueprintCallable)
		FVector GetMonsterPos();

	void SetID(const int& id);
};