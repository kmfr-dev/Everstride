// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIDefine.h"
#include "../../../Interface/AIInterface.h"
#include "../../CharacterBase.h"

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIOwner = OwnerComp.GetAIOwner();
	if (nullptr == AIOwner)
		return EBTNodeResult::Failed;

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (nullptr == BlackBoardComp)
		return EBTNodeResult::Failed;

	IAIInterface* AIPawn = Cast<IAIInterface>(AIOwner->GetPawn());
	if (nullptr == AIPawn)
		return EBTNodeResult::Failed;

	FAIAttackFinished OnAttackFinished;
	OnAttackFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	AIPawn->SetAIAttackDelegate(OnAttackFinished);
	AIPawn->AttackByAI();

	return EBTNodeResult::Type::InProgress;
}

void UBTTask_Attack::OnAttackFinished()
{
}
