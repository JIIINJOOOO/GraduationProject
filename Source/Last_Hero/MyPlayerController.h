// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Last_Hero.h"
#include "GameFramework/PlayerController.h"
#include "Network.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LAST_HERO_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	bool isBattle;
	bool isInput;

	bool isInput_W;
	bool isInput_A;
	bool isInput_S;
	bool isInput_D;
	bool isInput_Space;

	bool isInput_1;
	bool isInput_2;
	// high_resolutuon_clock::time_point a
	int input_1;
	int input_2;

	high_resolution_clock::time_point lastInputTime;


public:
	AMyPlayerController();

	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* aPawn) override;
	void ChangeInputMode(bool bGameMode = true);
	void ShowResultUI();

	virtual void Tick(float DeltaTime) override;
protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UABGameplayWidget> MenuWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI)
	TSubclassOf<class UMyGameplayResultWidget> ResultWidgetClass;

	
private:
	void OnGamePasuse();

	UPROPERTY()
	class UABGameplayWidget* MenuWidget;

	UPROPERTY()
	class UMyGameplayResultWidget* ResultWidget;

	FInputModeGameOnly GameInputMode;
	FInputModeUIOnly UIInputMode;
};
