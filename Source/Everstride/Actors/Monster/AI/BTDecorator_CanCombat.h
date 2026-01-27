// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_CanCombat.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API UBTDecorator_CanCombat : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_CanCombat();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
