// Fill out your copyright notice in the Description page of Project Settings.


#include "MyLoginTitleUserWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"

void UMyLoginTitleUserWidget::NativeConstruct()
{
	Super::NativeConstruct();
	LoginButton = Cast<UButton>(GetWidgetFromName(TEXT("LoginButton")));;
	UserNameTextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("NameInput")));

	LoginButton->OnClicked.AddDynamic(this, &UMyLoginTitleUserWidget::OnLoginClicked);
}

void UMyLoginTitleUserWidget::OnLoginClicked()
{
	FString UserName = UserNameTextBox->GetText().ToString();
	if (UserName.Len() <= 0 || UserName.Len() > 10) return;
}