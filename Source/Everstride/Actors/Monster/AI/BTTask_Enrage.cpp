// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Enrage.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../../../Component/SkillComponent.h"
#include "../BossMonsterBase.h"
#include "AIController.h"
#include "AIDefine.h"

EBTNodeResult::Type UBTTask_Enrage::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	//if (nullptr == BlackBoardComp)
	//	return EBTNodeResult::Failed;
	//
	//AAIController* ControllerPtr = OwnerComp.GetAIOwner();
	//if (nullptr == ControllerPtr)
	//	return EBTNodeResult::Failed;

	//ABossMonsterBase* BossPawn = ControllerPtr->GetPawn<ABossMonsterBase>();
	//if(nullptr == BossPawn)
	//	return EBTNodeResult::Failed;

	//FEnrageFinished OnEnrageFinished;
	//OnEnrageFinished.BindLambda(
	//	[&, BlackBoardComp]()
	//	{
	//		BlackBoardComp->SetValueAsBool(BBKEY_ENRAGED, true);
	//		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//	}
	//);

	//BossPawn->SetAIEnrageDelegate(OnEnrageFinished);
	//BossPawn->EnrageByAI();

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (nullptr == BlackBoardComp)
		return EBTNodeResult::Failed;


	AAIController* ControllerPtr = OwnerComp.GetAIOwner();
	if (nullptr == ControllerPtr)
		return EBTNodeResult::Failed;

	ACharacterBase* BossPawn = ControllerPtr->GetPawn<ACharacterBase>();
	if (nullptr == BossPawn)
		return EBTNodeResult::Failed;

	FSkillMappingData* SkillMappingData = BossPawn->GetSkillMappingData(EverstrideGamePlayTags::TAG_Skill_BossMonster_Roar);
	if (nullptr == SkillMappingData)
		return EBTNodeResult::Failed;

	USkillComponent* SkillComp = BossPawn->GetSkillComponent();
	if (nullptr == SkillComp)
		return EBTNodeResult::Failed;

	// 분노 상태로 변경
	BlackBoardComp->SetValueAsBool(BBKEY_ENRAGED, true);

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
