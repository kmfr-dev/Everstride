// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsTargetDead.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIDefine.h"
#include "../../CharacterBase.h"

UBTDecorator_IsTargetDead::UBTDecorator_IsTargetDead()
{
	NodeName = TEXT("Is Target Dead");

}

bool UBTDecorator_IsTargetDead::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (nullptr == BlackBoardComp)
		return false;

	ACharacterBase* Target = Cast<ACharacterBase>(BlackBoardComp->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == Target)
		return false;

	bResult = Target->IsDead();

	if (true == bResult)
		BlackBoardComp->SetValueAsObject(BBKEY_TARGET, nullptr);

	return bResult;
}
