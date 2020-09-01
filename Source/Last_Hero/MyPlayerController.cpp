// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "ABGameplayWidget.h"
#include "MyGameplayResultWidget.h"
#include "MyGameStateBase.h"
#include "Network.h"
extern Network net;

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
	
	PrimaryActorTick.bCanEverTick = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	bEnableMouseOverEvents = true;
	bEnableTouchOverEvents = true;

	isBattle = false;
	isInput = false;

	isInput_W = false;
	isInput_A = false;
	isInput_S = false;
	isInput_D = false;
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

void AMyPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	bool isPrevMove = false; // 이전 프레임에도 이동중이었는지
	if (net.isMoving) isPrevMove = true;
	else isPrevMove = false;

	// 키 하나가 여러 역할을 하는 경우는 나중에 처리
	if (IsInputKeyDown(EKeys::One)) {
		CS_WEAPON_ON pack;
		pack.size = sizeof(CS_WEAPON_ON);
		pack.type = cs_weapon_on;
		net.SendPacket(&pack);
	}
	if (IsInputKeyDown(EKeys::Two)) {
		CS_WEAPON_ON pack;
		pack.size = sizeof(CS_WEAPON_ON);
		pack.type = cs_weapon_on;
		net.SendPacket(&pack);
	}
	if (IsInputKeyDown(EKeys::Three)) {
		CS_BERSERK pack;
		pack.size = sizeof(CS_BERSERK);
		pack.type = cs_berserk;
		net.SendPacket(&pack);
	}
	if (IsInputKeyDown(EKeys::Eight)) {
		CS_FIREBALL pack;
		pack.size = sizeof(CS_FIREBALL);
		pack.type = cs_fireball;
		net.SendPacket(&pack);
	}
	if (IsInputKeyDown(EKeys::LeftMouseButton)) {
		CS_ATTACK pack;
		pack.size = sizeof(CS_ATTACK);
		pack.type = cs_attack;
		net.SendPacket(&pack);
	}
	if (IsInputKeyDown(EKeys::RightMouseButton)) {
		CS_GUARD pack;
		pack.size = sizeof(CS_GUARD);
		pack.type = cs_guard;
		net.SendPacket(&pack);
	}
	if (IsInputKeyDown(EKeys::SpaceBar)) {
		//if (isBattle) {
		CS_EVADE pack;
		pack.size = sizeof(CS_EVADE);
		pack.type = cs_evade;
		net.SendPacket(&pack);
		//}
		//else {
		//	CS_JUMP pack;
		//	pack.size = sizeof(CS_JUMP);
		//	pack.type = cs_jump;
		//	net.SendPacket(&pack);
		//}
	}
	if (IsInputKeyDown(EKeys::C)) {
		CS_HIDE pack;
		pack.size = sizeof(CS_HIDE);
		pack.type = cs_hide;
		net.SendPacket(&pack);
	}
	if (IsInputKeyDown(EKeys::E)) {
		CS_ASSASSIN pack;
		pack.size = sizeof(CS_ASSASSIN);
		pack.type = cs_assassin;
		net.SendPacket(&pack);
	}

	// For move packet
	if (IsInputKeyDown(EKeys::W))
		isInput_W = true;
	else isInput_W = false;
	if (IsInputKeyDown(EKeys::A))
		isInput_A = true;
	else isInput_A = false;
	if (IsInputKeyDown(EKeys::S))
		isInput_S = true;
	else isInput_S = false;
	if (IsInputKeyDown(EKeys::D))
		isInput_D = true;
	else isInput_D = false;
	if (IsInputKeyDown(EKeys::SpaceBar))
		isInput_Space = true;
	else isInput_Space = false;

	if (isInput_W || isInput_A || isInput_S || isInput_D || isInput_Space)
		net.isMoving = true;
	else net.isMoving = false;

	if (isPrevMove && !net.isMoving) {
		// 이전까지 이동하다가 이동이 중단되었을때. 즉, 이동종료 알림
		CS_MOVE_STOP pack{ sizeof(CS_MOVE_STOP), cs_move_stop };
		net.SendPacket(&pack);
	}
}