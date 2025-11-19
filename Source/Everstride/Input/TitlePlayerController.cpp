// Fill out your copyright notice in the Description page of Project Settings.


#include "TitlePlayerController.h"
#include "../UI/Title/TitleMainWidget.h"
#include "../Subsystem/UISubsystem.h"

void ATitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 메인 위젯 세팅
	UUISubsystem* UISub = UUISubsystem::Get(this);
	if (UISub)
		UISub->SetMainWidget();

	// 타이틀 레벨이므로 Input모드를 UIOnly로 변경하고 적용
	FInputModeUIOnly  InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	InputMode.SetWidgetToFocus(nullptr);
	SetShowMouseCursor(true);
	SetInputMode(InputMode);
}
