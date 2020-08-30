// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Last_Hero.h"
#include "GameFramework/Character.h"
#include "MyMonster.generated.h"

UCLASS()
class LAST_HERO_API AMyMonster : public ACharacter
{
	GENERATED_BODY()
private:
	int id;
	int hp;
public:
	// Sets default values for this character's properties
	AMyMonster();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		FVector MonPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterInfo)
		float ServerMonHP;
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