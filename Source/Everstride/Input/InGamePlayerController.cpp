// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePlayerController.h"

void AInGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 인게임 레벨이므로 Input모드를 Game으로 적용
	FInputModeGameOnly InputMode;
	SetShowMouseCursor(false);
	SetInputMode(InputMode);
}