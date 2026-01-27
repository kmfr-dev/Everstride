// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_AttackInRange.h"
#include "AIController.h"
#include "AIDefine.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../MonsterBase.h"

UBTDecorator_AttackInRange::UBTDecorator_AttackInRange()
{
	NodeName = TEXT("Can Attack");
}

bool UBTDecorator_AttackInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AAIController* AIOwner = OwnerComp.GetAIOwner();
	if (nullptr == AIOwner)
		return false;

	AMonsterBase* OwnerPawn = AIOwner->GetPawn<AMonsterBase>();
	if (nullptr == OwnerPawn)
		return false;

	// 블랙보드에서 Target액터를 얻어온다.
	APawn* TargetPawn = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == TargetPawn)
		return false;

	// 현재 AI가 조종하고있는 폰과 Target과의 거리를 구함
	float DistToTarget = OwnerPawn->GetDistanceTo(TargetPawn);

	// AI가 판단한 공격 범위내에 들어왔는지 판단
	float AttackRangeWithRadius = OwnerPawn->GetAttackRange();
	bResult = (DistToTarget <= AttackRangeWithRadius);

	return bResult;
}
