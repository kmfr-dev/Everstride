// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/IUserObjectListEntry.h"
#include "SkillSlot.generated.h"

UCLASS()
class EVERSTRIDE_API USkillSlot : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
	
protected:
	// 실제로 사용할 스킬 이미지 위젯
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mSkillImage = nullptr;

	// 머티리얼을 적용할 이미지 위젯
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UImage> mCoolDownImage = nullptr;

	// 쿨타임 텍스트
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UTextBlock> mCoolDownText = nullptr;

	float mSkillCooldownElapsed = 0.f;
	FTimerHandle mSkillCoolDownHandle;

	FNumberFormattingOptions CoolDownFormat;

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;

public:
	UFUNCTION()
	void SetCoolDown(float _CoolDown);

	UFUNCTION()
	void SetOpacity_CoolDownImage(float _NewOpacity);

	UFUNCTION()
	void SetOpacity_SkillImage(float _NewOpacity);

	UFUNCTION()
	void SetOpacity_CoolDownText(float _NewOpacity);

	UFUNCTION()
	void SetCoolDownText(float _CoolDown);

private:

	UFUNCTION()
	void ClearCoolDown();

	UFUNCTION()
	void EnableCoolDown(bool _Enable);
};
