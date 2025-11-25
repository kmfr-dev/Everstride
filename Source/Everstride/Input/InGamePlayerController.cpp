// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePlayerController.h"
#include "../Player/CharacterBase.h"
#include "../Player/Component/StatComponent.h"
#include "../Subsystem/UISubsystem.h"
#include "../UI/InGame/InGameMainWidget.h"

void AInGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 델리게이트 바인딩
	BindDelegate();

	// 인게임 레벨이므로 Input모드를 Game으로 적용
	FInputModeGameOnly InputMode;
	SetShowMouseCursor(false);
	SetInputMode(InputMode);
}

void AInGamePlayerController::BindDelegate()
{
	// 내 자신의 캐릭터를 얻어온다.
	ACharacterBase* CharacterPtr = GetPawn<ACharacterBase>();
	
	// 유효할 때만
	if (IsValid(CharacterPtr))
	{
		// 스탯 컴포넌트를 얻어와서 델리게이트에 함수 바인딩
		UStatComponent* StatComp = CharacterPtr->GetStatComponent();
		if (IsValid(StatComp))
			StatComp->OnStatUpdate.AddDynamic(this, &AInGamePlayerController::UpdateStatUI);


	}
}

void AInGamePlayerController::UpdateStatUI(EStatusType _Type, float _Value)
{
	UUISubsystem* UISub = UUISubsystem::Get(this);
	if (nullptr == UISub)
		return;
	
	// 메인 위젯을 얻어온다.
	UInGameMainWidget* MainWidget = Cast<UInGameMainWidget>(UISub->GetMainWidget());
	if (nullptr == MainWidget)
		return;

	switch (_Type)
	{
	case EStatusType::STATTYPE_HP:
		MainWidget->SetCurStatText(EStatBarType::STATBAR_TYPE_HP, _Value);
		MainWidget->SetStatBarPercent(EStatBarType::STATBAR_TYPE_HP, _Value);
		break;
	case EStatusType::STATTYPE_MAXHP:
		MainWidget->SetMaxStatText(EStatBarType::STATBAR_TYPE_HP, _Value);
		break;
	case EStatusType::STATTYPE_STAMINA:
		MainWidget->SetCurStatText(EStatBarType::STATBAR_TYPE_STAMINA, _Value);
		MainWidget->SetStatBarPercent(EStatBarType::STATBAR_TYPE_STAMINA, _Value);
		break;
	case EStatusType::STATTYPE_MAXSTAMINA:
		MainWidget->SetMaxStatText(EStatBarType::STATBAR_TYPE_STAMINA, _Value);
		break;
	case EStatusType::STATTYPE_RUNRATIO:
		break;
	case EStatusType::STATTYPE_ATTACK:
		break;
	}
}
