// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtonBase.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UButtonBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(mButton))
	{
		// 클릭시 호출할 함수 바인딩
		mButton->OnClicked.AddDynamic(this, &UButtonBase::OnClickedButtonEvent);
	}
}

void UButtonBase::OnClickedButtonEvent()
{
	// 델리게이트 변수에 함수가 바인딩 되어있을 때만
	// 해당 함수를 호출
	if (OnClickedButton.IsBound())
	{
		OnClickedButton.Broadcast();
	}
}

void UButtonBase::SetButtonText(const FString& _Text)
{
	mText->SetText(FText::FromString(_Text));
}
