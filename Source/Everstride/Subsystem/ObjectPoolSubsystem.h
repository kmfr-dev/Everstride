// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Common/GameDefines.h"
#include "Subsystems/WorldSubsystem.h"
#include "ObjectPoolSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API UObjectPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
private:
	// 오브젝트 풀링 맵
	UPROPERTY()
	TMap<TSubclassOf<AActor>, FPoolData> mPoolMap;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	// 특정 클래스 타입 오브젝트 풀 생성 함수
	UFUNCTION()
	void InitPool(TSubclassOf<AActor> _Class, int32 _PoolSize);

	// 사용한 풀링 액터를 풀에 반환하는 함수
	UFUNCTION()
	void ReturnToPool(AActor* _ReturnActor);

	// 특정 클래스 타입의 풀링된 액터를 얻어오는 함수
	UFUNCTION()
	AActor* GetActorFromPool(TSubclassOf<AActor> _Class);

	// 특정 클래스 타입이 현재 풀링되어있는지 확인하는 함수
	UFUNCTION()
	bool IsPooled(TSubclassOf<AActor> _Class);

public:
	static UObjectPoolSubsystem* Get(const UObject* _WorldContext);
};
