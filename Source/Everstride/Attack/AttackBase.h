// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Common/GameDefines.h"
#include "UObject/NoExportTypes.h"
#include "AttackBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class EVERSTRIDE_API UAttackBase : public UObject
{
	GENERATED_BODY()
	
public:
	virtual bool ExecuteAttack(ACharacter* _Owner);
};
