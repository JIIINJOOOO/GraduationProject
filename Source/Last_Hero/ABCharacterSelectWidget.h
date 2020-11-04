// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Last_Hero.h"
#include "Blueprint/UserWidget.h"
#include "ABCharacterSelectWidget.generated.h"

enum Weapon_Class
{
	Weapon_Sword, Weapon_Hammer
};
/**
 * 
 */
UCLASS()
class LAST_HERO_API UABCharacterSelectWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	UFUNCTION(BlueprintCallable)
		void NextCharacter(bool IsNextBtn);

	virtual void NativeConstruct() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		int32 CurIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
		int32 MaxIndex = 2;

	TWeakObjectPtr<USkeletalMeshComponent> TargetComponent;

	UPROPERTY()
		class UButton* PrevButton;

	UPROPERTY()
		class UButton* NextButton;

	UPROPERTY()
		class UEditableTextBox* TextBox;

	UPROPERTY()
		class UButton* ConfirmButton;

private:
	UFUNCTION()
		void OnPrevClicked();

	UFUNCTION()
		void OnNextClicked();

	UFUNCTION()
		void OnConfirmClicked();

};
