// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatComponent.h"
#include "PlayerCombatComponent.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API UPlayerCombatComponent : public UCombatComponent
{
	GENERATED_BODY()
	
public:
	UPlayerCombatComponent();

protected:
	// 공격 콤보 데이터 에셋
	UPROPERTY()
	TObjectPtr<class UComboActionData> mComboData = nullptr;

	// 현재 콤보 인덱스
	int32 mCurrentCombo = 0;
	// 콤보 타이머핸들
	FTimerHandle mComboTimerHandle;
	// 다음 콤보가 있는지 확인할 bool값
	bool mHasNextComboCommand = false;

public:
	virtual void AttackBegin() override;
	virtual void AttackHitCheck() override;
#pragma region ComboAttack
private:
	// 콤보 진행 함수
	UFUNCTION()
	void ProcessComboAttack();

	UFUNCTION()
	void ComboActionBegin();

	UFUNCTION()
	void ComboActionEnd(class UAnimMontage* _TargetMontage, bool _IsProperyEnded);

	UFUNCTION()
	void SetComboCheckTimer();

	UFUNCTION()
	void ComboCheck();

#pragma endregion ComboAttack
public:
	UFUNCTION()
	void TurnOwner();
};
