// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// 게임 재개 버튼
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButtonBase> mResume = nullptr;
	
	// 나가기 버튼
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButtonBase> mExit = nullptr;

protected:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnClickedResumeBtn();
	
	UFUNCTION()
	void OnClickedExitBtn();
};
