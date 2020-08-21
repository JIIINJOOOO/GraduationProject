// Fill out your copyright notice in the Description page of Project Settings.


#include "MyLoginTitleUserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"

extern Network net;

void UMyLoginTitleUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	LoginButton = Cast<UButton>(GetWidgetFromName(TEXT("LoginButton")));;
	UserNameTextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("NameInput")));
	UserPassTextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("PassInput")));

	LoginButton->OnClicked.AddDynamic(this, &UMyLoginTitleUserWidget::OnLoginClicked);
	
}

void UMyLoginTitleUserWidget::OnLoginClicked()
{
	bIsLogin = true;
	FString UserName = UserNameTextBox->GetText().ToString();
	FString UserPass = UserPassTextBox->GetText().ToString();
	if (UserName.Len() <= 0 || UserName.Len() > 10) return;
	if (UserPass.Len() <= 0 || UserPass.Len() > 10) return;
	net.SetAccountInfo((const char*)TCHAR_TO_ANSI(*UserName), (const char*)TCHAR_TO_ANSI(*UserPass));
	CS_LOGIN pack;
	pack.size = sizeof(CS_LOGIN);
	pack.type = login_packet;
	strcpy_s(pack.id, (char*)TCHAR_TO_ANSI(*UserName));
	strcpy_s(pack.password, (char*)TCHAR_TO_ANSI(*UserPass));
	net.SendPacket(&pack);
	int ret = WaitForSingleObject(net.loginEvent, 5000);
	if (ret == WAIT_OBJECT_0 && net.GetStatus() == p_login) {
		UE_LOG(LogTemp, Log, TEXT("SCENE CHANGE"));
		UGameplayStatics::OpenLevel(this, TransferLevelName);
	}
	// if (bIsLogin)
	// {
	// 	UGameplayStatics::OpenLevel(this, TransferLevelName);
	// }
}

DWORD WINAPI UMyLoginTitleUserWidget::LoginThread(LPVOID arg) {
	UMyLoginTitleUserWidget* lw = (UMyLoginTitleUserWidget*)arg;
	while (true) {
		// int ret = WaitForSingleObject(net.loginEvent, INFINITE);
		// if (ret == WAIT_OBJECT_0) {
		// 	UE_LOG(LogTemp, Log, TEXT("SCENE CHANGE"));
		// 	lw->ChangeScene();
		// 	break;
		// }
	}
	return 0;
}


void UMyLoginTitleUserWidget::ChangeScene() {
	UGameplayStatics::OpenLevel(this, TransferLevelName);
}
