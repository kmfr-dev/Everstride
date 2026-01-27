// Fill out your copyright notice in the Description page of Project Settings.

#include "StatComponent.h"
#include "../Actors/CharacterBase.h"
#include "../Actors/Player/InGamePlayerState.h"
#include "../Subsystem/TableSubsystem.h"

// Sets default values for this component's properties
UStatComponent::UStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UStatComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UStatComponent::InitStatus(const FName& _StatTID)
{
	// 테이블 서브시스템을 불러옴
	UTableSubsystem* TableSub = UTableSubsystem::Get(this);
	if (nullptr == TableSub)
		return;

	// 스탯 테이블 조회
	FStatTableRow* StatTable = TableSub->FindTableRow<FStatTableRow>(UTableData::TableName::STAT, _StatTID);
	if (nullptr == StatTable)
		return;

	// 해당 테이블의 능력치 데이터 배열을 순회 하면서
	// 스탯의 기본값을 설정해준다.
	for (const FDefaultStatData& Row : StatTable->Status)
	{
		// 맵에 해당 타입에 대응하는 정보가 있는지 확인,
		// 없으면 추가한다.
		if (!mStatus.Contains(Row.StatType))
		{
			mStatus.Add(Row.StatType, FStatData());
		}

		// 기본값과 토탈값 설정
		mStatus[Row.StatType].BaseStat = Row.Value;
		mStatus[Row.StatType].DeltaStat = 0.f;
		mStatus[Row.StatType].TotalStat = Row.Value;
	}
}

// Called every frame
void UStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UStatComponent::SetStatValue(EStatusType _StatType, EStatusCategory _StatCategory, float _Value)
{
	switch (_StatCategory)
	{
	case EStatusCategory::STATCATEGORY_BASE:
		mStatus[_StatType].BaseStat = _Value;
		break;
	case EStatusCategory::STATCATEGORY_DELTA:
		mStatus[_StatType].DeltaStat = _Value;
		break;
	case EStatusCategory::STATCATEGORY_TOTAL:
		mStatus[_StatType].TotalStat = _Value;
		break;
	}
}

float UStatComponent::GetTotalStatValue(EStatusType _StatType)
{
	if (nullptr == mStatus.Find(_StatType))
		return 0.f;

	return mStatus[_StatType].TotalStat;
}

float UStatComponent::GetStatValue(EStatusType _StatType, EStatusCategory _StatCategory)
{
	switch (_StatCategory)
	{
	case EStatusCategory::STATCATEGORY_BASE:
		return mStatus[_StatType].BaseStat;
	case EStatusCategory::STATCATEGORY_DELTA:
		return	mStatus[_StatType].DeltaStat;
	case EStatusCategory::STATCATEGORY_TOTAL:
		return	mStatus[_StatType].TotalStat;
	}

	return -1.f;
}

