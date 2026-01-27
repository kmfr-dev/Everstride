// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BossHPBar.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API UBossHPBar : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// 보스이름 텍스트 위젯
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> mBossName;
	
	// 현재 체력 텍스트 위젯
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> mHPText;
	
	// 체력바 위젯
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> mHPBar;

public:
	UFUNCTION()
	void SetBossName(const FString& _BossName);

	UFUNCTION()
	void SetHPPercent(float _NewPercent);

	UFUNCTION()
	void SetHPText(const FString& _Text);
};
