// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_TurnToTarget.h"
#include "AIController.h"
#include "AIDefine.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_TurnToTarget::UBTTask_TurnToTarget()
{
	NodeName = TEXT("Turn To Target");
}

EBTNodeResult::Type UBTTask_TurnToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIController* AIOwner = OwnerComp.GetAIOwner();
	if (nullptr == AIOwner)
		return EBTNodeResult::Failed;

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (nullptr == BlackBoardComp)
		return EBTNodeResult::Failed;

	APawn* OwnerPawn = AIOwner->GetPawn();
	if (nullptr == OwnerPawn)
		return EBTNodeResult::Failed;

	APawn* TargetPawn = Cast<APawn>(BlackBoardComp->GetValueAsObject(BBKEY_TARGET));
	if(nullptr == TargetPawn)
		return EBTNodeResult::Failed;

	const float TurnSpeed = 2.f;

	// 상대 위치벡터에서 나의 위치벡터를 빼면 상대를 향하는 벡터가 나온다.
	FVector LookVector = TargetPawn->GetActorLocation() - OwnerPawn->GetActorLocation();
	LookVector.Z = 0.0f;
	
	FRotator TargetRot = FRotationMatrix::MakeFromX(LookVector).Rotator();
	OwnerPawn->SetActorRotation(FMath::RInterpTo(OwnerPawn->GetActorRotation(), TargetRot, GetWorld()->GetDeltaSeconds(), TurnSpeed));



	return EBTNodeResult::Succeeded;
}
