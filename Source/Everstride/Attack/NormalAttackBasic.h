// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackBase.h"
#include "NormalAttackBasic.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class EVERSTRIDE_API UNormalAttackBasic : public UAttackBase
{
	GENERATED_BODY()
	
public:
	virtual bool ExecuteAttack(ACharacter* _Owner) override;
};
