// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Common/GameDefines.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateStatUI, EStatusType, _Type, float, _Value);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EVERSTRIDE_API UStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStatComponent();

#pragma region Variable

protected:
	// 캐릭터의 스탯 타입에 따라 기본값, 변경값, 최종값을 저장
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly)
	TMap<EStatusType, FStatData> mStatus;

public:
	// UI업데이트 용 델리게이트 변수
	UPROPERTY()
	FUpdateStatUI OnStatUpdate;

#pragma endregion Variable

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	// 테이블에서 스탯정보를 얻어와 초기화하는 함수
	UFUNCTION()
	void InitStatus();

	// 특정 스탯의 UI 업데이트 함수
	UFUNCTION()
	void InitStatusUI(EStatusType _Type);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 특정 스탯의 특정 데이터를 변경하는 함수
	// ex) HP스탯의 기본스텟 or 변경된 스탯 or 토탈스텟 변경
	void SetStatValue(EStatusType _StatType, EStatusCategory _StatCategory, float _Value);

	// 특정 스탯의 특정 데이터를 얻어오는 함수
	float GetStatValue(EStatusType _StatType, EStatusCategory _StatCategory);
		
};
