// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameplayResultWidget.h"
#include "Components/TextBlock.h"
#include "MyGameStateBase.h"

void UMyGameplayResultWidget::BindGameState(AMyGameStateBase* GameState)
{
	CurrentGameState = GameState;
}

void UMyGameplayResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	auto Result = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtResult")));
	Result->SetText(FText::FromString(CurrentGameState->IsGameCleared() ? TEXT("Clear") : TEXT("You Die")));
}

