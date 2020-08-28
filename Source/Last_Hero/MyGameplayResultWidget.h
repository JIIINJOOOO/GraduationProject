// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ABGameplayWidget.h"
#include "MyGameplayResultWidget.generated.h"

/**
 * 
 */
UCLASS()
class LAST_HERO_API UMyGameplayResultWidget : public UABGameplayWidget
{
	GENERATED_BODY()

public:
	void BindGameState(class AMyGameStateBase* GameState);

protected:
	virtual void NativeConstruct() override;

private:
	TWeakObjectPtr<class AMyGameStateBase> CurrentGameState;
	
};
