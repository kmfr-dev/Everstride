// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsTargetDead.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API UBTDecorator_IsTargetDead : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_IsTargetDead();
	
protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
};
