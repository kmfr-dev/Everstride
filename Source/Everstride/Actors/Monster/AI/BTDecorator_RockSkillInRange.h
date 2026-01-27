// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_RockSkillInRange.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API UBTDecorator_RockSkillInRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_RockSkillInRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
