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
	// 보스 UI
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UBossHPBar> mBossUI = nullptr;

	// 커스텀 체력바 위젯
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UStatBarWidget> mHPBar = nullptr;

	// 커스텀 스테미너바 위젯
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UStatBarWidget> mStaminaBar = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTileView> mSkillList = nullptr;
public:
	virtual void NativeConstruct() override;

public:
	// 초기 스킬 UI 세팅 함수
	UFUNCTION()
	void SetSkillSlotUI(const FName& _PlayerTID);

	// 스킬 사용 시 호출할 UI 업데이트 함수
	UFUNCTION()
	void UpdateSkillSlotUI(const FName& _SkillID);

	// 보스 UI 활성화 함수
	UFUNCTION()
	void SetEnableBossUI(bool _Enable);

	UFUNCTION()
	void SetBossName(const FString& _BossName);

	UFUNCTION()
	void SetBossHPBarPercent(float _NewPercent);

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
	
private:
	UFUNCTION()
	void SetSkillCoolDownUI(class USkillView* _Item, float _CoolDown);
};
