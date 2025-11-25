// Fill out your copyright notice in the Description page of Project Settings.

#include "StatComponent.h"
#include "../CharacterBase.h"
#include "../InGamePlayerState.h"
#include "../../Subsystem/TableSubsystem.h"

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

	InitStatus();
}

void UStatComponent::InitStatus()
{
	// 오너 액터를 얻어온다.
	ACharacterBase* OnwerPtr = GetOwner<ACharacterBase>();
	if (nullptr == OnwerPtr)
		return;

	// 액터의 플레이어 스테이트를 얻어온다.
	AInGamePlayerState* PlayerStatePtr = OnwerPtr->GetPlayerState<AInGamePlayerState>();
	if (nullptr == PlayerStatePtr)
		return;

	// 테이블 서브시스템을 불러옴
	UTableSubsystem* TableSub = UTableSubsystem::Get(this);
	if (nullptr == TableSub)
		return;

	// 플레이어 스테이트에서 Player테이블 ID를 얻어온다.
	FName PlayerTID = PlayerStatePtr->GetPlayerTID();
	
	// 플레이어 테이블 조회
	FPlayerTableRow* PlayerTable = TableSub->FindTableRow<FPlayerTableRow>(UTableData::TableName::PLAYER, PlayerTID);
	if (nullptr == PlayerTable)
		return;

	// 해당 테이블의 능력치 데이터 배열을 순회 하면서
	// 스탯의 기본값을 설정해준다.
	for (const FDefaultStatData& Row : PlayerTable->Status)
	{
		// 맵에 해당 타입에 대응하는 정보가 있는지 확인,
		// 없으면 추가한다.
		if (!mStatus.Contains(Row.StatType))
			mStatus.Add(Row.StatType, FStatData());

		// 기본값과 토탈값 설정
		mStatus[Row.StatType].BaseStat = Row.Value;
		mStatus[Row.StatType].DeltaStat = 0.f;
		mStatus[Row.StatType].TotalStat = Row.Value;

		InitStatusUI(Row.StatType);
	}
}

void UStatComponent::InitStatusUI(EStatusType _Type)
{
	float Value = mStatus[_Type].BaseStat;
	
	switch (_Type)
	{
	case EStatusType::STATTYPE_HP:
	{
		float MaxHP = mStatus[EStatusType::STATTYPE_MAXHP].BaseStat;
		Value /= MaxHP;
	}
		break;
	case EStatusType::STATTYPE_STAMINA:
	{
		float MaxStamina = mStatus[EStatusType::STATTYPE_MAXSTAMINA].BaseStat;
		Value /= MaxStamina;
	}
		break;
	}

	OnStatUpdate.Broadcast(_Type, Value);
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

