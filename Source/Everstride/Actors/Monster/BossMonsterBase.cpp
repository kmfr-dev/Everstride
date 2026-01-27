// Fill out your copyright notice in the Description page of Project Settings.


#include "BossMonsterBase.h"
#include "Controller/AIControllerBossMonster.h"
#include "../../Component/StatComponent.h"
#include "../../Animation/AnimInstanceBase.h"
#include "../../DataAsset/CharacterActionTag.h"
#include "../../Input/InGamePlayerController.h"
#include "../../Component/SkillComponent.h"

ABossMonsterBase::ABossMonsterBase()
{
	AIControllerClass = AAIControllerBossMonster::StaticClass();
}

void ABossMonsterBase::BeginPlay()
{
	Super::BeginPlay();

	SetActorScale3D(FVector(2.f));
}

void ABossMonsterBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	
	if(OnLandedFinished.IsBound())
	{
		OnLandedFinished.Execute();
		OnLandedFinished.Unbind();
	}
}

void ABossMonsterBase::UpdateHPBar(float _CurrentHP, float _MaxHP)
{
	const float NewPercent = _CurrentHP /= _MaxHP;
	
	if (false == OnChangedHP.IsBound())
		return;

	OnChangedHP.Execute(NewPercent);
	
	AAIControllerBossMonster* ControllerPtr = GetController<AAIControllerBossMonster>();
	if (nullptr == ControllerPtr)
		return;

	ControllerPtr->SetBossMonsterHP(NewPercent);
}

void ABossMonsterBase::Dead()
{
	Super::Dead();

	AInGamePlayerController* PlayerControllerPtr = GetWorld()->GetFirstPlayerController<AInGamePlayerController>();
	if (nullptr == PlayerControllerPtr)
		return;

	PlayerControllerPtr->EnableBossUI(false);


}

void ABossMonsterBase::AttackByAI()
{
	UAnimInstanceBase* AnimInstancePtr = Cast<UAnimInstanceBase>(GetMesh()->GetAnimInstance());
	if (nullptr == AnimInstancePtr)
		return;

	UAnimMontage* AttackMontage = AnimInstancePtr->GetAnimMontage(mTagData->AttackTag);
	if (nullptr == AttackMontage)
		return;

	// 몽타주 섹션 갯수를 얻어온다.
	int32 SectionNum = AttackMontage->GetNumSections() - 1;
	
	// 랜덤으로 인덱스를 하나 뽑는다.
	int32 RandomIndex = FMath::RandRange(0, SectionNum);

	// 해당 인덱스에 해당하는 섹션 이름을 얻어온다.
	FName SectionName = AttackMontage->GetSectionName(RandomIndex);

	// 몽타주 재생
	AnimInstancePtr->PlayAnimMontage(mTagData->AttackTag);

	// 해당 섹션으로 점프
	AnimInstancePtr->Montage_JumpToSection(SectionName, AttackMontage);

	FOnMontageEnded OnMontageEnded;
	OnMontageEnded.BindUObject(this, &ABossMonsterBase::AttackEnd);
	AnimInstancePtr->Montage_SetEndDelegate(OnMontageEnded);
}

void ABossMonsterBase::SetLandedDelegate(const FOnLanded& _InLanded)
{
	OnLandedFinished = _InLanded;
}

void ABossMonsterBase::SetBlackBoardDefaultHP()
{
	UStatComponent* StatComp = GetStatComponent();
	if (nullptr == StatComp)
		return;

	float CurHP = StatComp->GetTotalStatValue(EStatusType::STATTYPE_HP);
	const float MaxHP = StatComp->GetTotalStatValue(EStatusType::STATTYPE_MAXHP);

	CurHP /= MaxHP;

	AAIControllerBossMonster* ControllerPtr = GetController<AAIControllerBossMonster>();
	if (nullptr == ControllerPtr)
		return;

	ControllerPtr->SetBossMonsterHP(CurHP);
}

void ABossMonsterBase::SetAIEnrageDelegate(const FEnrageFinished& _InEnrageFinished)
{
	OnEnrageFinished = _InEnrageFinished;
}

void ABossMonsterBase::EnrageByAI()
{
	UAnimInstanceBase* AnimInstancePtr = Cast<UAnimInstanceBase>(GetMesh()->GetAnimInstance());
	if (nullptr == AnimInstancePtr)
		return;
	
	AnimInstancePtr->PlayAnimMontage(EverstrideGamePlayTags::TAG_AnimMontage_BossMonster_Enrage);

	FOnMontageEnded OnMontageEnded;
	OnMontageEnded.BindUObject(this, &ABossMonsterBase::EndEnraged);
	AnimInstancePtr->Montage_SetEndDelegate(OnMontageEnded);
}

void ABossMonsterBase::EndEnraged(UAnimMontage* _TargetMontage, bool _IsProperyEnded)
{
	UAnimInstanceBase* AnimInst = Cast<UAnimInstanceBase>(GetMesh()->GetAnimInstance());
	if (nullptr == AnimInst)
		return;

	UAnimMontage* MontagePtr = AnimInst->GetAnimMontage(EverstrideGamePlayTags::TAG_AnimMontage_BossMonster_Enrage);
	if (nullptr == MontagePtr)
		return;

	if (MontagePtr != _TargetMontage)
		return;

	OnEnrageFinished.Execute();
}

