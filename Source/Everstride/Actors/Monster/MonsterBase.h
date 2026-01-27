// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../CharacterBase.h"
#include "../../Interface/AIInterface.h"
#include "MonsterBase.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API AMonsterBase : public ACharacterBase, public IAIInterface
{
	GENERATED_BODY()

public:
	AMonsterBase();

protected:
	UPROPERTY()
	TObjectPtr<class UWidgetComponent> mHPBar = nullptr;

	UPROPERTY()
	FName mMonsterName = NAME_None;

	FAIAttackFinished OnAttackFinished;


protected:
	virtual void BeginPlay() override;

protected:
	virtual void DefaultSetting() override;
	virtual void UpdateHPBar(float _CurrentHP, float _MaxHP) override;
	virtual void AttackEnd(UAnimMontage* _TargetMontage, bool _IsProperyEnded);
	virtual void Dead() override;
	virtual void Hit(AActor* _HitActor) override;
	virtual void InitUI() override;

#pragma region InterfaceFunction Override
public:
	virtual void  SetAIAttackDelegate(const FAIAttackFinished& _InAttackFinished) override;
	virtual void  AttackByAI() override;
	virtual float GetAttackRange() override;
#pragma endregion

public:
	UFUNCTION()
	void EnableHPBar(bool _Enable);

public:
	const FName& GetMonsterName() { return mMonsterName; }
};
