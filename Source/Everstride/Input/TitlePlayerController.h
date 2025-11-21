// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerControllerBase.h"
#include "TitlePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API ATitlePlayerController : public APlayerControllerBase
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
};
