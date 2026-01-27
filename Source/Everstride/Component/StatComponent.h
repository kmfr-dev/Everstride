// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Common/GameDefines.h"
#include "Components/ActorComponent.h"
#include "StatComponent.generated.h"


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

#pragma endregion Variable

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION()
	void InitStatus(const FName& _StatTID);

	// 특정 스탯의 특정 데이터를 변경하는 함수
	// ex) HP스탯의 기본스텟 or 변경된 스탯 or 토탈스텟 변경
	void SetStatValue(EStatusType _StatType, EStatusCategory _StatCategory, float _Value);

	UFUNCTION()
	float GetTotalStatValue(EStatusType _StatType);

	// 특정 스탯의 특정 데이터를 얻어오는 함수
	UFUNCTION(BlueprintCallable)
	float GetStatValue(EStatusType _StatType, EStatusCategory _StatCategory);
		
};
