// Fill out your copyright notice in the Description page of Project Settings.

#include "ABCharacterSelectWidget.h"
#include "MyGameInstance.h"
#include "EngineUtils.h"
#include "Animation/SkeletalMeshActor.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"

void UABCharacterSelectWidget::NextCharacter(bool IsNextBtn)
{
	auto MyGameInstance = GetWorld()->GetGameInstance<UMyGameInstance>();
	ABCHECK(nullptr != MyGameInstance);
	ABCHECK(TargetComponent.IsValid());

	if (IsNextBtn)
	{
		if (CurIndex < MaxIndex - 1)
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
	//switch (CurIndex)
	//{
	//case /*Weapon_Sword*/0:
	//	//TargetComponent.Get()->GetChildComponent(0)->SetVisibility(true, false); // 소드
	//	//TargetComponent.Get()->GetChildComponent(1)->SetVisibility(true, false); // shield
	//	//TargetComponent.Get()->GetChildComponent(2)->SetVisibility(false, false); // 해머

	//	break;
	//case /*Weapon_Hammer*/1:
	//	//TargetComponent.Get()->GetChildComponent(0)->SetVisibility(false, false); // 소드
	//	//TargetComponent.Get()->GetChildComponent(1)->SetVisibility(false, false); // shield
	//	//TargetComponent.Get()->GetChildComponent(2)->SetVisibility(true, false); // 해머
	//	break;
	//default:
	//	break;
	//}
	MyGameInstance->CharTypeNum = CurIndex;

	//for (TActorIterator<ASkeletalMeshActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	//{
	//	if (ActorItr)
	//	{
	//		auto CharacterSkeletalMesh = Cast<ASkeletalMeshActor>(*ActorItr);
	//		
	//		//CharacterSkeletalMesh->GetSkeletalMeshComponent()->GetAttachChildren()->Find("SwordSM")->SetVisibility(true);
	//	}
	//}

	
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
	auto MyGameInstance = GetWorld()->GetGameInstance<UMyGameInstance>();
	MyGameInstance->CharTypeNum = CurIndex;
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("Test_map2"));
}

