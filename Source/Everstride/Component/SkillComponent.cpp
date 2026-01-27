// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillComponent.h"
#include "../Subsystem/TableSubsystem.h"
#include "../Subsystem/ObjectPoolSubsystem.h"
#include "../Skill/SkillBase.h"
#include "../Animation/AnimInstanceBase.h"

// Sets default values for this component's properties
USkillComponent::USkillComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USkillComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USkillComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 매틱마다 스킬 데이터 순회
	for (auto& it : mSkillDatas)
	{
		// 현재 요소가 시전중인 스킬이면 continue
		if (it.TID == mCastSkill)
			continue;

		// 현재 요소의 현재 쿨타임이 설정한 쿨타임보다 크거나 같으면 continue
		if (it.CurCoolDown >= it.CoolDown)
			continue;

		// 쿨타임시간 누적
		it.CurCoolDown += DeltaTime;
	}
}

void USkillComponent::SetSkillData(const FName& _ID)
{
	UTableSubsystem* TableSub = UTableSubsystem::Get(this);
	if (nullptr == TableSub)
		return;

	FSkillTableRow* SkillRow = TableSub->FindTableRow<FSkillTableRow>(UTableData::TableName::SKILL, _ID);
	if (nullptr == SkillRow)
		return;

	FSkillData* FoundSkill = FindSkill(_ID);
	if (FoundSkill)
		return;

	UObjectPoolSubsystem* PoolSub = UObjectPoolSubsystem::Get(this);
	if (nullptr == PoolSub)
		return;

	// 풀링해야하는 스킬이면 오브젝트 풀링 요청
	if(ESkillPoolingType::SKILLPOLLING_TYPE_POOLED == SkillRow->SkillPoolType)
		PoolSub->InitPool(SkillRow->SkillClass, POOLSIZE);

	// 스킬 데이터 배열에 추가
	mSkillDatas.Emplace(FSkillData(_ID, SkillRow->CoolDown));
}

bool USkillComponent::ChooseSkill(const FName& _ID)
{
	if (mCastSkill == _ID)
		return false;

	FSkillData* FoundSkill = FindSkill(_ID);
	if (nullptr == FoundSkill)
		return false;

	// 현재 고른스킬이 쿨타임이면 고르지 못하게 return
	if (FoundSkill->CoolDown > FoundSkill->CurCoolDown)
		return false;

	mCastSkill = _ID;

	return true;
}

bool USkillComponent::SkillAction(const FName& _ID)
{
	if (mCastSkill != _ID)
		return false;

	// 스킬을 찾아서 쿨타임을 검증.
	FSkillData* FoundSkill = FindSkill(mCastSkill);
	if (nullptr == FoundSkill)
		return false;
	
	if (FoundSkill->CoolDown > FoundSkill->CurCoolDown)
		return false;

	ACharacterBase* OwnerPtr = GetOwner<ACharacterBase>();
	if (nullptr == OwnerPtr)
		return false;

	UAnimInstanceBase* AnimInst = Cast<UAnimInstanceBase>(OwnerPtr->GetMesh()->GetAnimInstance());
	if (nullptr == AnimInst)
		return false;

	UTableSubsystem* TableSub = UTableSubsystem::Get(this);
	if (nullptr == TableSub)
		return false;

	FSkillTableRow* SkillRow = TableSub->FindTableRow<FSkillTableRow>(UTableData::TableName::SKILL, mCastSkill);
	if (nullptr == SkillRow)
		return false;

	// 애님 몽타주에서 SkillActionStart를 호출
	AnimInst->PlayAnimMontage(SkillRow->SkillTag);
	
	OnSkillActionEnd.BindUObject(this, &USkillComponent::SkillActionEnd);
	// 몽타주가 끝났을 때 호출할 함수 바인딩
	AnimInst->Montage_SetEndDelegate(OnSkillActionEnd);

	// 즉발 스킬이라면 바로 스킬을 사용
	if (ESkillActionType::SKILLACTION_TYPE_INSTANT == SkillRow->SkillActionType)
	{
		SkillActionStart();
	}
	else
	{
		SkillActionPreStart();
	}

	return true;
}

void USkillComponent::SkillActionPreStart()
{
}

void USkillComponent::SkillActionStart()
{
	ACharacterBase* OwnerPtr = Cast<ACharacterBase>(GetOwner());
	if (nullptr == OwnerPtr)
		return;

	UTableSubsystem* TableSub = UTableSubsystem::Get(this);
	if (nullptr == TableSub)
		return;

	FSkillTableRow* SkillRow = TableSub->FindTableRow<FSkillTableRow>(UTableData::TableName::SKILL, mCastSkill);
	if (nullptr == SkillRow)
		return;

	ASkillBase* Skill = nullptr;
	bool SuccessUsedSkill = false;

	// 스킬 풀여부에 따라 스킬 실행
	switch (SkillRow->SkillPoolType)
	{
	case ESkillPoolingType::SKILLPOLLING_TYPE_NONPOOLED:
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		Skill = GetWorld()->SpawnActor<ASkillBase>(SkillRow->SkillClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
		
		if (nullptr == Skill)
			return;
		
		SuccessUsedSkill = Skill->ExecuteSkill(OwnerPtr, mCastSkill);
	}
		break;
	case ESkillPoolingType::SKILLPOLLING_TYPE_POOLED:
	{
		// 오브젝트 풀 서브시스템에 접근
		UObjectPoolSubsystem* PoolSub = UObjectPoolSubsystem::Get(this);
		if (nullptr == PoolSub)
			return;
		
		// 풀링된 스킬 액터를 받아온다.
		Skill = Cast<ASkillBase>(PoolSub->GetActorFromPool(SkillRow->SkillClass));
		if (nullptr == Skill)
			return;

		// 스킬 사용, 실패시 풀로 리턴
		SuccessUsedSkill = Skill->ExecuteSkill(OwnerPtr, mCastSkill);

		if (!SuccessUsedSkill)
		{
			PoolSub->ReturnToPool(Skill);
			return;
		}
	}
		break;
	}

	if (!SuccessUsedSkill)
	{
		SkillEnd();
	}

	else if (Skill)
	{
		mCastingSkillActor = Skill;
	}
}

void USkillComponent::SkillEnd()
{
	if (OnSkillFinished.IsBound())
	{
		OnSkillFinished.Execute();
	}

	RefreshSkillCoolDown(mCastSkill);
	ClearCastSkill();
}

void USkillComponent::SkillActionEnd(UAnimMontage* _Montage, bool _Cancel)
{
	if (_Cancel)
		return;

	// 문제
	// 1. A몽타주 재생중 히트 몽타주 재생 -> mCastSkill이 있어서 통과돼서 스킬이 종료됨 A스킬이 종료됨
	// 2. 그렇다고 false == IsNone  return을 하면 포효끝나고 스킬종료가안됨

	ACharacterBase* OwnerPtr = GetOwner<ACharacterBase>();
	if (nullptr == OwnerPtr)
		return;

	UAnimInstanceBase* AnimInst = Cast<UAnimInstanceBase>(OwnerPtr->GetMesh()->GetAnimInstance());
	if (nullptr == AnimInst)
		return;

	UTableSubsystem* TableSub = UTableSubsystem::Get(this);
	if (nullptr == TableSub)
		return;

	FSkillTableRow* SkillRow = TableSub->FindTableRow<FSkillTableRow>(UTableData::TableName::SKILL, mCastSkill);
	if (nullptr == SkillRow)
		return;
	
	UAnimMontage* TargetMontage = AnimInst->GetAnimMontage(SkillRow->SkillTag);
	if (nullptr == TargetMontage)
		return;

	if (false == mCastSkill.IsNone())
	{
		if (_Montage != TargetMontage)
			return;
	}
	

	// 만약 스킬 ID가 있다면
	/*if (_Montage != TargetMontage || false == mCastSkill.IsNone())
	{
		return;
	}*/

	if (false == SkillRow->SkillEndByLogic)
	{
		SkillEnd();
	}
}

void USkillComponent::RefreshSkillCoolDown(const FName& _ID)
{
	FSkillData* FoundSkill = FindSkill(_ID);
	if (nullptr == FoundSkill)
		return;

	FoundSkill->CurCoolDown = 0.f;
}

FSkillData* USkillComponent::FindSkill(const FName& _ID)
{
	return mSkillDatas.FindByKey(_ID);
}

const FName& USkillComponent::GetCastingSkill()
{
	return mCastSkill;
}

bool USkillComponent::IsSameSkill(const FName& _ID)
{
	if (mCastSkill == _ID)
		return true;

	return false;
}

ASkillBase* USkillComponent::GetCastingSkillActor()
{
	return mCastingSkillActor;
}

void USkillComponent::SetSkillFinished(const FOnSkillFinished& _InSkillFinished)
{
	OnSkillFinished = _InSkillFinished;
}

const TArray<FSkillData>& USkillComponent::GetSkillDatas()
{
	return mSkillDatas;
}

void USkillComponent::ClearCastSkill()
{	
	mCastSkill = NAME_None;
	mCastingSkillActor = nullptr;
}

