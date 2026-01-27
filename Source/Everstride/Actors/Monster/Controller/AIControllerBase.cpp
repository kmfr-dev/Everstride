// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackBoardData.h"

AAIControllerBase::AAIControllerBase()
{
	
}

void AAIControllerBase::RunTree()
{
	if (IsValid(mAITree) && IsValid(mBoardData))
	{
		UBlackboardComponent* BlackBoardComp = nullptr;
		if (true == UseBlackboard(mBoardData, BlackBoardComp))
			RunBehaviorTree(mAITree);
	}
}

void AAIControllerBase::StopBehaviorTree()
{
	if (nullptr == mAITree)
		return;

	UBehaviorTreeComponent* BTComp = Cast<UBehaviorTreeComponent>(BrainComponent);
	if (nullptr == BTComp)
		return;

	BTComp->StopTree(EBTStopMode::Forced);
}

void AAIControllerBase::SetGenericTeamId(const FGenericTeamId& _TeamID)
{
	mTeamID = _TeamID;
}

FGenericTeamId AAIControllerBase::GetGenericTeamId() const
{
	return mTeamID;
}
