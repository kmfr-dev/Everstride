// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillBase.h"
#include "../Subsystem/ObjectPoolSubsystem.h"
#include "../Actors/CharacterBase.h"
#include "../Component/SkillComponent.h"
#include "../Subsystem/TableSubsystem.h"

// Sets default values
ASkillBase::ASkillBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASkillBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASkillBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ASkillBase::ExecuteSkill(ACharacter* _Owner, const FName& _ID)
{
	if (nullptr == _Owner)
		return false;

	mSkillCaster = _Owner;
	mSkillID = _ID;

	return true;
}

const float ASkillBase::GetSkillValue() const
{
	float RetVal = 0.f;

	UTableSubsystem* TableSub = UTableSubsystem::Get(this);
	if (nullptr == TableSub)
		return RetVal;

	FSkillTableRow* SkillRow = TableSub->FindTableRow<FSkillTableRow>(UTableData::TableName::SKILL, mSkillID);
	if (nullptr == SkillRow)
		return RetVal;

	RetVal = SkillRow->Value;

	return RetVal;
}

void ASkillBase::EndSkill()
{
	ACharacterBase* SkillCaster = Cast<ACharacterBase>(mSkillCaster);
	if (nullptr == SkillCaster)
		return;

	USkillComponent* SkillComp = SkillCaster->GetSkillComponent();
	if (nullptr == SkillComp)
		return;

	SkillComp->SkillEnd();

	// 풀서브시스템을 얻어와 스킬 자기자신을 풀에 반납한다.
	UObjectPoolSubsystem* PoolSub = UObjectPoolSubsystem::Get(this);
	if (nullptr == PoolSub)
		return;

	PoolSub->ReturnToPool(this);
}

void ASkillBase::OnSpawnFromPool_Implementation()
{
}

void ASkillBase::OnReturnToPool_Implementation()
{
}
