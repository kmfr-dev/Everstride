// Fill out your copyright notice in the Description page of Project Settings.


#include "StatBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UStatBarWidget::SetStatPercent(float _NewPercent)
{
	if (IsValid(mStatBar))
		mStatBar->SetPercent(_NewPercent);
}

void UStatBarWidget::SetCurrentStatText(float _StatValue)
{
	if (IsValid(mCurStatText))
		mCurStatText->SetText(FText::AsNumber(_StatValue));
}

void UStatBarWidget::SetMaxStatText(float _StatValue)
{
	if (IsValid(mMaxStatText))
		mMaxStatText->SetText(FText::AsNumber(_StatValue));
}

void UStatBarWidget::SetStatBarColor(EStatBarType _Type)
{
	if (false == IsValid(mStatBar))
		return;

	FLinearColor ProgressBarColor;

	switch (_Type)
	{
		break;
	case EStatBarType::STATBAR_TYPE_HP:
		ProgressBarColor = FLinearColor::Red;
		break;
	case EStatBarType::STATBAR_TYPE_STAMINA:
		ProgressBarColor = FLinearColor::Yellow;
		break;
	case EStatBarType::STATBAR_TYPE_NONE:
	default:
		ProgressBarColor = FLinearColor::Black;
	}

	mStatBar->SetFillColorAndOpacity(ProgressBarColor);
}
