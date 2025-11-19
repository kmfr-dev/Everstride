// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "../Table/TableData.h"
#include "TableSubsystem.generated.h"


inline UDataTable* TABLE_LOAD(FString _path);

UCLASS()
class EVERSTRIDE_API UTableSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	// 데이터 테이블들을 맵으로 관리
	UPROPERTY()
	TMap<FName, UDataTable*> mTables;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
	// 모든 데이터 테이블을 로드하는 함수
	UFUNCTION()
	bool TableLoad();

public:
	// 특정 데이터 테이블을 반환하는 함수
	UFUNCTION()
	UDataTable* FindTable(FName _TableName);

	// 특정 데이터 테이블의 특정 행을 반환하는 함수
	template<typename T>
	T* FindTableRow(FName _TableName, FName _RowName)
	{
		UDataTable* FoundTable = FindTable(_TableName);
		if (nullptr == FoundTable)
			return nullptr;

		return FoundTable->FindRow<T>(_RowName, "Not Found Table Row...");
	}


public:
	static UTableSubsystem* Get(const UObject* _WorldContext);
};
