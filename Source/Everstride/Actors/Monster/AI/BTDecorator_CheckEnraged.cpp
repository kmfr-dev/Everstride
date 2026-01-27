// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckEnraged.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIDefine.h"

UBTDecorator_CheckEnraged::UBTDecorator_CheckEnraged()
{
	NodeName = TEXT("Check Enraged");
}

bool UBTDecorator_CheckEnraged::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (nullptr == BlackBoardComp)
		bResult = false;

	bResult = BlackBoardComp->GetValueAsBool(BBKEY_ENRAGED);

	return bResult;
}
