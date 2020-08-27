// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "ABGameplayWidget.h"
#include "MyGameplayResultWidget.h"
#include "MyGameStateBase.h"

AMyPlayerController::AMyPlayerController()
{
	static ConstructorHelpers::FClassFinder< UABGameplayWidget> UI_MENU_C(TEXT("/Game/Game/UI/UI_Menu.UI_Menu_C"));
	if (UI_MENU_C.Succeeded())
	{
		MenuWidgetClass = UI_MENU_C.Class;
	}


	static ConstructorHelpers::FClassFinder< UMyGameplayResultWidget> UI_RESULT_C(TEXT("/Game/Game/UI/UI_Result.UI_Result_C"));
	if (UI_RESULT_C.Succeeded())
	{
		ResultWidgetClass = UI_RESULT_C.Class;
	}

}

void AMyPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void AMyPlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction(TEXT("GamePause"), EInputEvent::IE_Pressed, this, &AMyPlayerController::OnGamePasuse);
}

void AMyPlayerController::ChangeInputMode(bool bGameMode)
{
	if (bGameMode)
	{
		SetInputMode(GameInputMode);
		bShowMouseCursor = false;
	}
	else
	{
		SetInputMode(UIInputMode);
		bShowMouseCursor = true;
	}
}


void AMyPlayerController::OnGamePasuse()
{
	MenuWidget = CreateWidget< UABGameplayWidget>(this, MenuWidgetClass);
	MenuWidget->AddToViewport(1);

	SetPause(true);
	ChangeInputMode(false);
}


void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ChangeInputMode(true);
	ResultWidget = CreateWidget< UMyGameplayResultWidget>(this, ResultWidgetClass);

}

void AMyPlayerController::ShowResultUI()
{
	auto  AmyGameState = Cast<AMyGameStateBase> ( UGameplayStatics::GetGameState(this));
	ResultWidget->BindGameState(AmyGameState);

	ResultWidget->AddToViewport();
	ChangeInputMode(false);
}