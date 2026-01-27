// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttackBase.h"
#include "RangeNormalAttack.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class EVERSTRIDE_API URangeNormalAttack : public UAttackBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> mProjectileClass = nullptr;
	
public:
	virtual bool ExecuteAttack(ACharacter* _Owner) override;
};
