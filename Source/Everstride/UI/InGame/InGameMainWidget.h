// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameMainWidget.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API UInGameMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	// 커스텀 체력바 위젯
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UStatBarWidget> mHPBar;

	// 커스텀 스테미너바 위젯
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UStatBarWidget> mStaminaBar;

public:
	virtual void NativeConstruct() override;

public:
	// ====== 프로그레스바 퍼센트 Setter ======
	UFUNCTION()
	void SetStatBarPercent(EStatBarType _Type, float _NewPercent);
	
	UFUNCTION()
	void SetHPBarPercent(float _NewPercent);

	UFUNCTION()
	void SetStaminaBarPercent(float _NewPercent);
	// ========================================

	// ====== 스텟 텍스트 Setter ======
	UFUNCTION()
	void SetCurStatText(EStatBarType _Type, float _StatVal);

	UFUNCTION()
	void SetCurHPBarText(float _StatVal);

	UFUNCTION()
	void SetCurStaminaBarText(float _StatVal);

	UFUNCTION()
	void SetMaxStatText(EStatBarType _Type, float _StatVal);
	
	UFUNCTION()
	void SetMaxHPText(float _StatVal);

	UFUNCTION()
	void SetMaxStaminaText(float _StatVal);
	// ================================
	
};
