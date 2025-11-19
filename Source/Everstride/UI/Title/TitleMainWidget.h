// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleMainWidget.generated.h"

UCLASS()
class EVERSTRIDE_API UTitleMainWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	// 게임 시작 버튼
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<class UButtonBase> mGameStart;

	// 게임 나가기 버튼
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UButtonBase> mExit;

public:
	virtual void NativeConstruct() override;

public:
	// 게임 시작 버튼 클릭 이벤트 함수
	UFUNCTION()
	void OnClickedGameStartBtn();

	// 게임 나가기 버튼 클릭 이벤트 함수
	UFUNCTION()
	void OnClickedExitBtn();
};
