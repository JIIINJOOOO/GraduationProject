// Fill out your copyright notice in the Description page of Project Settings.


#include "MyLoginTitleUserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Network.h"

Network g_network;

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
	FString UserName = UserNameTextBox->GetText().ToString();
	FString UserPass = UserPassTextBox->GetText().ToString();
	if (UserName.Len() <= 0 || UserName.Len() > 10) return;
	if (UserPass.Len() <= 0 || UserPass.Len() > 10) return;

	g_network.SetAccountInfo((const char*)TCHAR_TO_ANSI(*UserName), (const char*)TCHAR_TO_ANSI(*UserPass));
	CS_LOGIN pack;
	pack.size = sizeof(CS_LOGIN);
	pack.type = login_packet;
	strcpy(pack.id, (char*)TCHAR_TO_ANSI(*UserName));
	strcpy(pack.password, (char*)TCHAR_TO_ANSI(*UserPass));
	g_network.SendPacket(&pack);
	// 씬 전환 여기서 해야함
	// if (g_network.GetStatus() == p_login) {
	// 	UE_LOG(LogTemp, Log, TEXT("SCENE CHANGE"));
	// }
}