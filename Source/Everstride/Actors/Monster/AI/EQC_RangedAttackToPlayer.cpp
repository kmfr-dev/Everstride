// Fill out your copyright notice in the Description page of Project Settings.


#include "EQC_RangedAttackToPlayer.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"
#include "AIDefine.h"
#include "AIController.h"

void UEQC_RangedAttackToPlayer::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	Super::ProvideContext(QueryInstance, ContextData);

	APawn* OwnerPawn = Cast<APawn>(QueryInstance.Owner.Get());
	if (nullptr == OwnerPawn)
		return;

	AAIController* OwnerController = OwnerPawn->GetController<AAIController>();
	if (nullptr == OwnerController)
		return;

	UBlackboardComponent* BlackBoardComp = OwnerController->GetBlackboardComponent();
	if (nullptr == BlackBoardComp)
		return;

	APawn* TargetPawn = Cast<APawn>(BlackBoardComp->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == TargetPawn)
		return;

	
	UEnvQueryItemType_Actor::SetContextHelper(ContextData, TargetPawn);
}
