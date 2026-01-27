// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CanCombat.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIDefine.h"

UBTDecorator_CanCombat::UBTDecorator_CanCombat()
{
	NodeName = TEXT("CanCombat");
}

bool UBTDecorator_CanCombat::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (nullptr == BlackBoardComp)
	{
		bResult = false;
		return bResult;
	}

	bResult = BlackBoardComp->GetValueAsBool(BBKEY_CANCOMBAT);

	return bResult;
}

