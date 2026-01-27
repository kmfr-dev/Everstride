// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_RockSkillInRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "../../CharacterBase.h"
#include "AIDefine.h"
#include "../../../Subsystem/TableSubsystem.h"

UBTDecorator_RockSkillInRange::UBTDecorator_RockSkillInRange()
{
	NodeName = TEXT("Target In Rock Skill Range.");
}

bool UBTDecorator_RockSkillInRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (nullptr == BlackBoardComp)
		bResult = false;

	AAIController* AIOwner = OwnerComp.GetAIOwner();
	if(nullptr == AIOwner)
		bResult = false;

	ACharacterBase* AIPawn = AIOwner->GetPawn<ACharacterBase>();
	if (nullptr == AIPawn)
		bResult = false;
	
	FSkillMappingData* MappingData = AIPawn->GetSkillMappingData(EverstrideGamePlayTags::TAG_Skill_BossMonster_ThrowRock);
	if(nullptr == MappingData)
		bResult = false;

	UTableSubsystem* TableSub = UTableSubsystem::Get(AIPawn);
	if(nullptr == TableSub)
		bResult = false;

	FSkillTableRow* SkillRow = TableSub->FindTableRow<FSkillTableRow>(UTableData::TableName::SKILL, MappingData->SkillID);
	if (nullptr == SkillRow)
		bResult = false;

	const float SkillRange = SkillRow->MaxRange;
	const float TargetDist = BlackBoardComp->GetValueAsFloat(BBKEY_TARGETDIST);
	
	bResult = (SkillRange >= TargetDist);

	return bResult;
}
