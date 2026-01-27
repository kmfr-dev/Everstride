// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdateTargetDist.h"
#include "AIDefine.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "AIController.h" 
#include "../MonsterBase.h"

UBTService_UpdateTargetDist::UBTService_UpdateTargetDist()
{
	NodeName = TEXT("Update Target Distance");
}

void UBTService_UpdateTargetDist::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (nullptr == BlackBoardComp)
		return;

	AAIController* AIOwner= OwnerComp.GetAIOwner();
	if (nullptr == AIOwner)
		return;

	AMonsterBase* AIPawn = AIOwner->GetPawn<AMonsterBase>();
	if (nullptr == AIPawn)
		return;

	AActor* Target = Cast< AActor>(BlackBoardComp->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == Target)
		return;

	const float TargetDist = AIPawn->GetDistanceTo(Target);

	BlackBoardComp->SetValueAsFloat(BBKEY_TARGETDIST, TargetDist);
}
