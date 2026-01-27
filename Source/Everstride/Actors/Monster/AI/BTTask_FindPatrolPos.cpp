// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPos.h"
#include "AIDefine.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UBTTask_FindPatrolPos::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	AAIController* AIOwner = OwnerComp.GetAIOwner();

	if(nullptr == AIOwner)
		return EBTNodeResult::Failed;

	APawn* OnwerPawn = AIOwner->GetPawn();
	if (nullptr == OnwerPawn)
		return EBTNodeResult::Failed;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(OnwerPawn->GetWorld());
	if (nullptr == NavSystem)
		return EBTNodeResult::Failed;

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (nullptr == BlackBoardComp)
		return EBTNodeResult::Failed;

	FVector Origin = BlackBoardComp->GetValueAsVector(BBKEY_HOMEPOS);
	FNavLocation NextPatrolPos;

	if (NavSystem->GetRandomPointInNavigableRadius(Origin, PATROLRADIUS, NextPatrolPos))
	{
		BlackBoardComp->SetValueAsVector(BBKEY_PATROLPOS, NextPatrolPos.Location);
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Failed;
}
