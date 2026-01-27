// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "GroundSmash.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API AGroundSmash : public ASkillBase
{
	GENERATED_BODY()

protected:
	// 도약 속도
	UPROPERTY(EditAnyWhere)
	float mLanchSpeed = 0.f;

	// 도약 시 설정할 중력값
	UPROPERTY(EditAnyWhere)
	float mGravityZ = 0.f;

	// 내려찍을 때 범위값
	UPROPERTY(EditAnyWhere)
	float mHitRange = 0.f;

protected:
	virtual void BeginPlay() override;

public:
	virtual bool ExecuteSkill(ACharacter* _Owner, const FName& _ID);

public:
	// 스킬 판정 처리 함수
	UFUNCTION()
	void SkillHitCheck();

protected:
	// 도약 종료시 호출될 함수
	UFUNCTION()
	void GroundSmashEnd();
	
	UFUNCTION()
	void IgnoreCollisionToTarget(ACharacter* _Target, bool _Enable);
};
