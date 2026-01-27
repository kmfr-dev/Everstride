// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckDistance.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "AIDefine.h"

UBTDecorator_CheckDistance::UBTDecorator_CheckDistance()
{
	NodeName = TEXT("Check Score Min Distance");
}

bool UBTDecorator_CheckDistance::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	AAIController* AIOwner = OwnerComp.GetAIOwner();
	if (nullptr == AIOwner)
		return EBTNodeResult::Failed;

	UBlackboardComponent* BlackBoardComp = AIOwner->GetBlackboardComponent();
	if (nullptr == BlackBoardComp)
		return EBTNodeResult::Failed;

	APawn* AIPawn = AIOwner->GetPawn();
	if (nullptr == AIPawn)
		return EBTNodeResult::Failed;

	AActor* TargetActor = Cast<AActor>(BlackBoardComp->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == TargetActor)
		return EBTNodeResult::Failed;

	// 타겟 액터와의 거리를 구함
	float Distance = AIPawn->GetDistanceTo(TargetActor);

	// EQS 공격 최소거리를 얻어옴.
	float EQSMinDis = BlackBoardComp->GetValueAsFloat(BBKEY_EQS_DISTANCE_MIN);

	// 만약 공격 최소 거리보다 멀리있는 경우 유효한 거리로 간주
	bResult = Distance > EQSMinDis;

	BlackBoardComp->SetValueAsInt(BBKEY_EQS_SCORINGFACTOR, !bResult);
	
	return bResult;
}