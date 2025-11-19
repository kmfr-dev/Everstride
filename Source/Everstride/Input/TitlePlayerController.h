// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TitlePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API ATitlePlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	TObjectPtr<class UTitleMainWidget> mMainWidget;

public:
	virtual void BeginPlay() override;
};
