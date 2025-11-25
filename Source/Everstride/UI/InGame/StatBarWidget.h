// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../Common/GameDefines.h"
#include "Blueprint/UserWidget.h"
#include "StatBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API UStatBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	// 스탯 프로그레스바 위젯
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> mStatBar;

	// 현재 스텟 표시 텍스트
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> mCurStatText;

	// 최대 스텟 표시 텍스트
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> mMaxStatText;

public:
	// 프로그레스바 퍼센트 Setter
	UFUNCTION()
	void SetStatPercent(float _NewPercent);

	// 현재 스텟 텍스트 Setter
	UFUNCTION()
	void SetCurrentStatText(float _StatValue);

	// 최대 스텟 텍스트 Setter
	UFUNCTION()
	void SetMaxStatText(float _StatValue);

	UFUNCTION()
	void SetStatBarColor(EStatBarType _Type);
};
