// Fill out your copyright notice in the Description page of Project Settings.

#include "ABCharacterSelectWidget.h"
#include "MyGameInstance.h"
#include "EngineUtils.h"
#include "Animation/SkeletalMeshActor.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"

void UABCharacterSelectWidget::NextCharacter(bool IsNextBtn)
{
	if (IsNextBtn)
	{
		if (CurIndex < MaxIndex)
		{
			CurIndex++;
		}
	}
	else // prevbtn
	{
		if (CurIndex > 0)
		{
			CurIndex--;
		}
	}
	switch (CurIndex)
	{
	case Weapon_Sword:
		TargetComponent.Get()->GetChildComponent(1)->SetVisibility(true, false); // sword 
		TargetComponent.Get()->GetChildComponent(2)->SetVisibility(true, false); // shield
		TargetComponent.Get()->GetChildComponent(0)->SetVisibility(false, false); // hammer
		break;
	case Weapon_Hammer:
		TargetComponent.Get()->GetChildComponent(1)->SetVisibility(false, false); // sword 
		TargetComponent.Get()->GetChildComponent(2)->SetVisibility(false, false); // shield
		TargetComponent.Get()->GetChildComponent(0)->SetVisibility(true, false); // hammer
		break;
	case Weapon_3:
		break;
	case Weapon_4:
		break;
	default:
		break;
	}

	//for (TActorIterator<ASkeletalMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	//{
	//	if (ActorItr)
	//	{
	//		auto CharacterSkeletalMesh = Cast<ASkeletalMeshActor>(*ActorItr);
	//		
	//		//CharacterSkeletalMesh->GetSkeletalMeshComponent()->GetAttachChildren()->Find("SwordSM")->SetVisibility(true);
	//	}
	//}
	auto MyGameInstance = GetWorld()->GetGameInstance<UMyGameInstance>();
	ABCHECK(nullptr != MyGameInstance);
	ABCHECK(TargetComponent.IsValid());

	
	/*if (nullptr != Asset)
	{
		TargetComponent->SetSkeletalMesh(Asset);
	}*/
}

void UABCharacterSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	/*CurrentIndex = 0;
	auto CharacterSetting = GetDefault<UABCharacterSettings>();
	MaxIndex = CharacterSetting->CharacterAssets.Num();
*/
	for (TActorIterator<ASkeletalMeshActor> It(GetWorld()); It; ++It)
	{
		TargetComponent = It->GetSkeletalMeshComponent();
		break;
	}

	PrevButton = Cast<UButton>(GetWidgetFromName(TEXT("btnPrev")));
	ABCHECK(nullptr != PrevButton);

	NextButton = Cast<UButton>(GetWidgetFromName(TEXT("btnNext")));
	ABCHECK(nullptr != NextButton);

	//TextBox = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("edtPlayerName")));
	//ABCHECK(nullptr != TextBox);

	ConfirmButton = Cast<UButton>(GetWidgetFromName(TEXT("btnConfirm")));
	ABCHECK(nullptr != ConfirmButton);

	PrevButton->OnClicked.AddDynamic(this, &UABCharacterSelectWidget::OnPrevClicked);
	NextButton->OnClicked.AddDynamic(this, &UABCharacterSelectWidget::OnNextClicked);
	ConfirmButton->OnClicked.AddDynamic(this, &UABCharacterSelectWidget::OnConfirmClicked);
}

void UABCharacterSelectWidget::OnPrevClicked()
{
	NextCharacter(false);
}

void UABCharacterSelectWidget::OnNextClicked()
{
	NextCharacter(true);
}

void UABCharacterSelectWidget::OnConfirmClicked()
{
	// 여기서 플레이어 아이디 정해주거나 그런 동작

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Test_map2"));
}

