// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Common/GameDefines.h"
#include "GameFramework/Actor.h"
#include "../Interface/ObjectPoolInterface.h"

#include "SkillBase.generated.h"

UCLASS()
class EVERSTRIDE_API ASkillBase : public AActor, public IObjectPoolInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillBase();

protected:
	// 스킬 시전자 캐릭터를 참조하기 위한 변수
	UPROPERTY()
	TObjectPtr<ACharacter> mSkillCaster = nullptr;
	
	// 스킬 자신의 TID
	UPROPERTY()
	FName mSkillID = NAME_None;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	virtual bool ExecuteSkill(ACharacter* _Owner, const FName& _ID);

	UFUNCTION()
	const float GetSkillValue() const;

	UFUNCTION()
	void EndSkill();

	UFUNCTION()
	virtual void OnSpawnFromPool_Implementation() override;

	UFUNCTION()
	virtual void OnReturnToPool_Implementation() override;

};
