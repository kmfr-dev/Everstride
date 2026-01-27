// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Common/GameDefines.h"
#include "Engine/DataAsset.h"
#include "CharacterActionTag.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API UCharacterActionTag : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Name)
	FGameplayTag AttackTag = FGameplayTag::EmptyTag;
	
	UPROPERTY(EditAnywhere, Category = Name)
	FGameplayTag DeathTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditAnywhere, Category = Name)
	FGameplayTag HitTag = FGameplayTag::EmptyTag;
};
