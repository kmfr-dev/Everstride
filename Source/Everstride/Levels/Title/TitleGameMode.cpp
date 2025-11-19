// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleGameMode.h"
#include "../../Input/TitlePlayerController.h"
#include "../../EverstrideGameInstance.h"

ATitleGameMode::ATitleGameMode()
{
	DefaultPawnClass = nullptr;
	PlayerControllerClass = ATitlePlayerController::StaticClass();
}

void ATitleGameMode::BeginPlay()
{
	// 게임 인스턴스를 얻어와 레벨타입 설정
	UEverstrideGameInstance* GameInst = GetGameInstance<UEverstrideGameInstance>();
	if (GameInst)
		GameInst->SetLevelType(ELevelType::LEVEL_TYPE_TITLE);
}
