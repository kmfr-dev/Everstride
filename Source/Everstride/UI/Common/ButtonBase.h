// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ButtonBase.generated.h"

// 버튼 클릭 이벤트에 사용할 델리게이트
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickedButton);

// 전방 선언
class UButton;
class UTextBlock;

// 공통적으로 사용할 버튼 클래스
UCLASS()
class EVERSTRIDE_API UButtonBase : public UUserWidget
{
	GENERATED_BODY()
	
private:
	// 버튼 위젯
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> mButton;

	// 텍스트 블록 위젯
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> mText;

public:
	// 버튼 클릭 델리게이트 변수
	FOnClickedButton OnClickedButton;

public:
	virtual void NativeConstruct() override;

public:
	// 버튼 클릭 이벤트 함수
	UFUNCTION()
	void OnClickedButtonEvent();

public:
	// 텍스트 Setter
	UFUNCTION()
	void SetButtonText(const FString& _Text);
};