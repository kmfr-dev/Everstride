// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Common/GameDefines.h"
#include "Engine/GameInstance.h"
#include "EverstrideGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API UEverstrideGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	ELevelType mLevelType = ELevelType::LEVEL_TYPE_NONE;

public:
	void SetLevelType(ELevelType _Type) { mLevelType = _Type; }
	ELevelType GetLevelType() { return mLevelType; }
};
