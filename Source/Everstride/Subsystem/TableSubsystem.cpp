// Fill out your copyright notice in the Description page of Project Settings.


#include "TableSubsystem.h"

void UTableSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (false == TableLoad())
	{
		
		return;
	}
}

bool UTableSubsystem::TableLoad()
{
	UDataTable* TableLoadPath = TABLE_LOAD(FString(TEXT("/Game/DataTable/DT_TableLoad")));
	if (nullptr == TableLoadPath)
		return false;

	bool Result = true;

	TableLoadPath->ForeachRow<FTableLoadRow>
		(TEXT("Not Found Table Row.."), [this, &Result](const FName& _Key, const FTableLoadRow& _Value)
			{
				// 해당 행의 로드 여부를 확인하고 맵에 추가
				if (_Value.IsLoad)
				{
					UDataTable* LoadTable = TABLE_LOAD(_Value.TablePath);
					if (true == IsValid(LoadTable))
						mTables.Add(_Key, LoadTable);
					else
						Result = false;
				}
			});

	return false;
}

UDataTable* UTableSubsystem::FindTable(FName _TableName)
{
	if (mTables.Find(_TableName) == nullptr)
		return nullptr;

	return *mTables.Find(_TableName);
}

UTableSubsystem* UTableSubsystem::Get(const UObject* _WorldContext)
{
	if(nullptr == _WorldContext)
		return nullptr;

	UGameInstance* GameInst = UGameplayStatics::GetGameInstance(_WorldContext);
	if (nullptr == GameInst)
		return nullptr;

	return GameInst->GetSubsystem<UTableSubsystem>();
}

inline UDataTable* TABLE_LOAD(FString _path)
{
	return Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *_path));
}
