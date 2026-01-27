// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnManager.h"
#include "../../Subsystem/TableSubsystem.h"
#include "../Monster/MonsterBase.h"

// Sets default values
ASpawnManager::ASpawnManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnMonster();
}

// Called every frame
void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnManager::SpawnMonster()
{
	UTableSubsystem* TableSub = UTableSubsystem::Get(this);
	if (nullptr == TableSub)
		return;

	FMapTableRow* MapRow = TableSub->FindTableRow<FMapTableRow>(UTableData::TableName::MAP, mMapName);
	if (nullptr == MapRow)
		return;

	for (const FName& TID : MapRow->SpawnTIDs)
	{
		FSpawnTableRow* SpawnRow = TableSub->FindTableRow<FSpawnTableRow>(UTableData::TableName::SPAWN, TID);
		if (nullptr == SpawnRow)
			continue;
		
		FMonsterTableRow* MonsterRow = TableSub->FindTableRow<FMonsterTableRow>
			(UTableData::TableName::MONSTER, SpawnRow->SpawnObjectTID);
		if (nullptr == MonsterRow)
			continue;

		AMonsterBase* Monster = GetWorld()->SpawnActorDeferred<AMonsterBase>
			(MonsterRow->MonsterClass, FTransform(SpawnRow->SpawnRotation, SpawnRow->SpawnPosition), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
		Monster->SetCharacterTID(SpawnRow->SpawnObjectTID);
		Monster->FinishSpawning(Monster->GetActorTransform());
	}
}

