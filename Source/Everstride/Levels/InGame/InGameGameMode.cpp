// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameGameMode.h"
#include "../../Actors/Player/PlayerBase.h"
#include "../../Actors/Player/InGamePlayerState.h"
#include "../../Input/InGamePlayerController.h"
#include "../../EverstrideGameInstance.h"

AInGameGameMode::AInGameGameMode()
{
	DefaultPawnClass = APlayerBase::StaticClass();
	PlayerControllerClass = AInGamePlayerController::StaticClass();
	PlayerStateClass = AInGamePlayerState::StaticClass();
	SpectatorClass = nullptr;
}

void AInGameGameMode::BeginPlay()
{
	Super::BeginPlay();

	UEverstrideGameInstance* GameInst = GetGameInstance<UEverstrideGameInstance>();
	if (GameInst)
		GameInst->SetLevelType(ELevelType::LEVEL_TYPE_INGAME);
}
