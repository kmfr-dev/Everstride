// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerMonster.h"
#include "BehaviorTree/BehaviorTree.h" 
#include "BehaviorTree/BlackBoardData.h"
#include "BehaviorTree/BlackBoardComponent.h"
#include "Perception/AIPerceptionComponent.h"	//ai 인지 컴포넌트 헤더
#include "Perception/AISenseConfig_Sight.h"		//ai 시각 자극 헤더
#include "../MonsterBase.h"
#include "../../../Subsystem/TableSubsystem.h"
#include "../AI/AIDefine.h"
#include "../../../Component/StatComponent.h"

AAIControllerMonster::AAIControllerMonster()
{
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	//AIPerception에 감각을 추가
	AIPerception->ConfigureSense(*SightConfig);

	//대표 감각기관을 지정
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());


}

void AAIControllerMonster::BeginPlay()
{
	Super::BeginPlay();

	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &AAIControllerMonster::OnTargetFound);
}

void AAIControllerMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAIControllerMonster::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AMonsterBase* MonsterPtr = Cast<AMonsterBase>(InPawn);
	if (false == IsValid(MonsterPtr))
		return;

	UTableSubsystem* TableSub = UTableSubsystem::Get(this);
	if (nullptr == TableSub)
		return;

	const FName& MonsterTID = MonsterPtr->GetCharacterTID();

	FMonsterTableRow* MonsterRow = TableSub->FindTableRow<FMonsterTableRow>(UTableData::TableName::MONSTER, MonsterTID);
	if (nullptr == MonsterRow)
		return;

	FAITableRow* AIRow = TableSub->FindTableRow<FAITableRow>(UTableData::TableName::AI, MonsterRow->AITID);
	if (nullptr == AIRow)
		return;

	mAITree = AIRow->Tree;
	mBoardData = AIRow->Board;

	UAISenseConfig_Sight* SightPtr = PerceptionComponent->GetSenseConfig<UAISenseConfig_Sight>();
	if (IsValid(SightPtr))
	{
		// AI 시야 반경 설정
		SightPtr->SightRadius = AIRow->SightRadius;
		// 상실 거리 설정
		SightPtr->LoseSightRadius = AIRow->LoseSightRadius;
		// 반각 설정
		SightPtr->PeripheralVisionAngleDegrees = AIRow->PeripheralVisionAngleDegrees;
		// 적군 감지여부설정
		SightPtr->DetectionByAffiliation.bDetectEnemies = AIRow->bDetectEnemies;
		// 우리팀 감지여부 설정
		SightPtr->DetectionByAffiliation.bDetectFriendlies = AIRow->bDetectFriendlies;
		// 중립팀 감지 여부 설정
		SightPtr->DetectionByAffiliation.bDetectNeutrals = AIRow->bDetectNeutrals;

		PerceptionComponent->ConfigureSense(*SightConfig);
	}

	// 팀아이디 설정
	SetGenericTeamId(MonsterRow->TeamID);

	if (false == bIsRunTree)
		return;

	if (IsValid(AIRow->Tree) && IsValid(AIRow->Board))
	{
		UBlackboardComponent* BlackBoardComp = nullptr;
		if (true == UseBlackboard(AIRow->Board, BlackBoardComp))
		{
			Blackboard->SetValueAsVector(BBKEY_HOMEPOS, GetPawn()->GetActorLocation());
			RunBehaviorTree(mAITree);
		}
	}
}

void AAIControllerMonster::OnUnPossess()
{
	Super::OnUnPossess();
}

void AAIControllerMonster::SetBlackBoardData()
{
	if (nullptr == Blackboard)
		return;

	AMonsterBase* MonsterPtr = GetPawn<AMonsterBase>();
	if (nullptr == MonsterPtr)
		return;

	UStatComponent* StatComp = MonsterPtr->GetStatComponent();
	if (nullptr == StatComp)
		return;

	float AttackRange = StatComp->GetTotalStatValue(EStatusType::STATTYPE_ATTACKRANGE);
	
	
	Blackboard->SetValueAsInt(BBKEY_EQS_SCORINGFACTOR, 1);
	Blackboard->SetValueAsFloat(BBKEY_ATTACKRANGE, AttackRange);
	Blackboard->SetValueAsFloat(BBKEY_EQS_DISTANCE_MIN, AttackRange * 0.5);
	Blackboard->SetValueAsFloat(BBKEY_EQSRANGE, AttackRange * 2);
}

void AAIControllerMonster::SetTarget(AActor* _TargetActor)
{
	Blackboard->SetValueAsObject(TEXT("Target"), _TargetActor);
}

AActor* AAIControllerMonster::GetTarget()
{
	AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(BBKEY_TARGET));
	if (Target)
		return Target;

	return nullptr;
}

void AAIControllerMonster::OnTargetFound(AActor* Actor, FAIStimulus Stimulus)
{
	if (nullptr == Blackboard || nullptr == Actor)
		return;

	AMonsterBase* AIPawn = GetPawn<AMonsterBase>();
	if (nullptr == AIPawn)
		return;

	// 타겟을 발견했을 시 호출되는 함수
	ETeamAttitude::Type TeamType = GetTeamAttitudeTowards(*Actor);

	// 적군이면서, 감지를 성공햇을 경우
	if (ETeamAttitude::Type::Hostile ==  TeamType &&
		true == Stimulus.WasSuccessfullySensed())
	{
		// 태그로 확인 해보니 플레이어 라면 타겟 갱신
		if (Actor->ActorHasTag("Player"))
		{
			SetTarget(Actor);
			AIPawn->EnableHPBar(true);
		}
	}

	// 감지 실패시
	else
	{
		AActor* FoundTarget = Cast<AActor>(Blackboard->GetValueAsObject(TEXT("Target")));
		if (true == IsValid(FoundTarget))
		{
			//자기 메모리를 지운다
			AIPerception->ForgetAll();
			SetTarget(nullptr);
			AIPawn->EnableHPBar(false);
		}
	}
}