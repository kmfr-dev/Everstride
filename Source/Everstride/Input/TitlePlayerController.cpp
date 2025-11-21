// Fill out your copyright notice in the Description page of Project Settings.


#include "TitlePlayerController.h"

void ATitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 타이틀 레벨이므로 Input모드를 UIOnly로 변경하고 적용
	FInputModeUIOnly  InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	InputMode.SetWidgetToFocus(nullptr);
	SetShowMouseCursor(true);
	SetInputMode(InputMode);
}
