// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GroundSmash.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../CharacterBase.h"
#include "../../../Component/SkillComponent.h"
#include "AIDefine.h"

EBTNodeResult::Type UBTTask_GroundSmash::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (nullptr == BlackBoardComp)
		return EBTNodeResult::Failed;

	AAIController* ControllerPtr = OwnerComp.GetAIOwner();
	if (nullptr == ControllerPtr)
		return EBTNodeResult::Failed;

	ACharacterBase* BossPawn = ControllerPtr->GetPawn<ACharacterBase>();
	if (nullptr == BossPawn)
		return EBTNodeResult::Failed;

	FSkillMappingData* SkillMappingData = BossPawn->GetSkillMappingData(EverstrideGamePlayTags::TAG_Skill_BossMonster_GrondLand);
	if (nullptr == SkillMappingData)
		return EBTNodeResult::Failed;

	USkillComponent* SkillComp = BossPawn->GetSkillComponent();
	if (nullptr == SkillComp)
		return EBTNodeResult::Failed;

	// 스킬을 선택한다.
	// 쿨타임일 경우에도 false 리턴이므로 실패시 Failed,
	bool ChooseSkill = SkillComp->ChooseSkill(SkillMappingData->SkillID);
	if (false == ChooseSkill)
		return EBTNodeResult::Failed;

	// 스킬 종료후 호출할 델리게이트를 스킬컴포넌트에 설정
	FOnSkillFinished OnSkillFinished;
	OnSkillFinished.BindLambda(
		[&]()
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	);

	SkillComp->SetSkillFinished(OnSkillFinished);

	// 스킬 사용
	bool IsUsed = SkillComp->SkillAction(SkillMappingData->SkillID);
	if (false == IsUsed)
		return EBTNodeResult::Failed;

	return EBTNodeResult::InProgress;
}
