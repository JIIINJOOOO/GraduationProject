// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyLoginTitleUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class LAST_HERO_API UMyLoginTitleUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "My LoginTitle User Widget")
		FString MyLoginTitleUserWidget;
	void NativeConstruct();
protected:
	UPROPERTY()
		class UButton* LoginButton;
	UPROPERTY()
		class UEditableTextBox* UserNameTextBox;
	UPROPERTY()
		class UEditableTextBox* UserPassTextBox;

private:
	UFUNCTION()
		void OnLoginClicked();
};
