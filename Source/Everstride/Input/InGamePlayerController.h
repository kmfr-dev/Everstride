// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerControllerBase.h"
#include "GameFramework/PlayerController.h"
#include "InGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API AInGamePlayerController : public APlayerControllerBase
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

private:
	// UI와 캐릭터 연동 함수
	UFUNCTION()
	void BindDelegate();

	UFUNCTION()
	void UpdateStatUI(EStatusType _Type, float _Value);

};
