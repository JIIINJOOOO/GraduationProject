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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterInfo)
		float ServerDamage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MonsterInfo)
		float ServerRecovery;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		int RandAttack;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Server)
		bool ready_cpp;

	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetID(const int& id);
};
