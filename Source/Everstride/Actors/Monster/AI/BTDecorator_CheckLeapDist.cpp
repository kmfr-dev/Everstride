// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_CheckLeapDist.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIDefine.h"
#include "AIController.h"
#include "../../../Component/SkillComponent.h"
#include "../../CharacterBase.h"
#include "../../../Subsystem/TableSubsystem.h"

UBTDecorator_CheckLeapDist::UBTDecorator_CheckLeapDist()
{
	NodeName = TEXT("Check Leap Dist");
}

bool UBTDecorator_CheckLeapDist::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (nullptr == BlackBoardComp)
	{
		bResult = false;
		return bResult;
	}

	AAIController* AIOwner = OwnerComp.GetAIOwner();
	if (nullptr == AIOwner)
	{
		bResult = false;
		return bResult;
	}

	ACharacterBase* AIPawn = AIOwner->GetPawn<ACharacterBase>();
	if (nullptr == AIPawn)
	{
		bResult = false;
		return bResult;
	}

	FSkillMappingData* SkillMappingData = AIPawn->GetSkillMappingData(EverstrideGamePlayTags::TAG_Skill_BossMonster_GrondLand);
	if (nullptr == SkillMappingData)
	{
		bResult = false;
		return bResult;
	}
	
	UTableSubsystem* TableSub = UTableSubsystem::Get(AIPawn);
	if (nullptr == TableSub)
	{
		bResult = false;
		return bResult;
	}

	FSkillTableRow* SkillRow = TableSub->FindTableRow<FSkillTableRow>(UTableData::TableName::SKILL, SkillMappingData->SkillID);
	if (nullptr == SkillRow)
	{
		bResult = false;
		return bResult;
	}
	
	// 현재 타겟과의 거리를 얻어온다.
	const float TargetDist = BlackBoardComp->GetValueAsFloat(BBKEY_TARGETDIST);
	// 스킬 범위
	const float MinRange = SkillRow->MinRange;
	const float MaxRange = SkillRow->MaxRange;
	
	bResult = (TargetDist <= MaxRange && TargetDist >= MinRange);

	return bResult;
}
