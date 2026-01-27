#include "BTService_UpdateCombat.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../MonsterBase.h"
#include "../../../Component/SkillComponent.h"
#include "AIController.h"
#include "AIDefine.h"
#include "../../../Subsystem/TableSubsystem.h"

void UBTService_UpdateCombat::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	if (nullptr == BlackBoardComp)
		return;

	AAIController* AIOwner = OwnerComp.GetAIOwner();
	if (nullptr == AIOwner)
		return;

	AMonsterBase* AIPawn = AIOwner->GetPawn<AMonsterBase>();
	if (nullptr == AIPawn)
		return;

	USkillComponent* SkillComp = AIPawn->GetSkillComponent();
	if (nullptr == SkillComp)
		return;

	bool IsCombat = false;
	
	const float TargetDist = OwnerComp.GetBlackboardComponent()->GetValueAsFloat(BBKEY_TARGETDIST);
	const float AttackRangeWithRadius = AIPawn->GetAttackRange();
	
	// 타겟이 기본 공격 사거리 안이라면 전투 가능상태
	if (TargetDist <= AttackRangeWithRadius)
		IsCombat = true;
	
	// 스킬 정보를 얻어온다.
	const TArray<FSkillData>& SkillDatas = SkillComp->GetSkillDatas();
	for (const FSkillData& SkillData : SkillDatas)
	{
		// 쿨타임 돌은 스킬이 있다면
		if (SkillData.CurCoolDown >= SkillData.CoolDown)
		{
			UTableSubsystem* TableSub = UTableSubsystem::Get(AIPawn);
			if (nullptr == TableSub)
				continue;

			FSkillTableRow* SkillRow = TableSub->FindTableRow<FSkillTableRow>(UTableData::TableName::SKILL, SkillData.TID);
			if (nullptr == SkillRow)
				continue;

			// 스킬 범위 안이라면 
			if (TargetDist >= SkillRow->MinRange && 
				TargetDist <= SkillRow->MaxRange)
				IsCombat = true;
		}
	}

	BlackBoardComp->SetValueAsBool(BBKEY_CANCOMBAT, IsCombat);
}
