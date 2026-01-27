// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonsterBase.h"
#include "BossMonsterBase.generated.h"


DECLARE_DELEGATE(FEnrageFinished);
DECLARE_DELEGATE(FOnLanded);
DECLARE_DELEGATE_OneParam(FOnChangedHP, float);

UCLASS()
class EVERSTRIDE_API ABossMonsterBase : public AMonsterBase
{
	GENERATED_BODY()
	
public:
	ABossMonsterBase();

public:
	FOnLanded OnLandedFinished;
	FEnrageFinished OnEnrageFinished;
	FOnChangedHP OnChangedHP;
	
protected:
	virtual void BeginPlay() override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void UpdateHPBar(float _CurrentHP, float _MaxHP) override;
	virtual void Dead() override;
	virtual void AttackByAI() override;
	

public:
	void SetLandedDelegate(const FOnLanded& _InLanded);
	void SetBlackBoardDefaultHP();
	void SetAIEnrageDelegate(const FEnrageFinished& _InEnrageFinished);
	UFUNCTION()
	void EnrageByAI();

	UFUNCTION()
	void EndEnraged(UAnimMontage* _TargetMontage, bool _IsProperyEnded);
};
