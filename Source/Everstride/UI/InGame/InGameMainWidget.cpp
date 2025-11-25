// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMainWidget.h"
#include "StatBarWidget.h"

void UInGameMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(mHPBar))
		mHPBar->SetStatBarColor(EStatBarType::STATBAR_TYPE_HP);

	if (IsValid(mStaminaBar))
		mStaminaBar->SetStatBarColor(EStatBarType::STATBAR_TYPE_STAMINA);
}

void UInGameMainWidget::SetStatBarPercent(EStatBarType _Type, float _NewPercent)
{
	switch (_Type)
	{
	case EStatBarType::STATBAR_TYPE_HP:
		SetHPBarPercent(_NewPercent);
		break;
	case EStatBarType::STATBAR_TYPE_STAMINA:
		SetStaminaBarPercent(_NewPercent);
		break;
	}
}

void UInGameMainWidget::SetHPBarPercent(float _NewPercent)
{
	if (IsValid(mHPBar))
		mHPBar->SetStatPercent(_NewPercent);
}

void UInGameMainWidget::SetStaminaBarPercent(float _NewPercent)
{
	if (IsValid(mStaminaBar))
		mStaminaBar->SetStatPercent(_NewPercent);
}

void UInGameMainWidget::SetCurStatText(EStatBarType _Type, float _StatVal)
{
	switch (_Type)
	{
	case EStatBarType::STATBAR_TYPE_HP:
		SetCurHPBarText(_StatVal);
		break;
	case EStatBarType::STATBAR_TYPE_STAMINA:
		SetCurStaminaBarText(_StatVal);
		break;
	}
}

void UInGameMainWidget::SetCurHPBarText(float _StatVal)
{
	if (IsValid(mHPBar))
		mHPBar->SetCurrentStatText(_StatVal);
}

void UInGameMainWidget::SetCurStaminaBarText(float _StatVal)
{
	if (IsValid(mHPBar))
		mStaminaBar->SetCurrentStatText(_StatVal);
}

void UInGameMainWidget::SetMaxStatText(EStatBarType _Type, float _StatVal)
{
	switch (_Type)
	{
	case EStatBarType::STATBAR_TYPE_HP:
		SetMaxHPText(_StatVal);
		break;
	case EStatBarType::STATBAR_TYPE_STAMINA:
		SetMaxStaminaText(_StatVal);
		break;
	}
}

void UInGameMainWidget::SetMaxHPText(float _StatVal)
{
	if (IsValid(mHPBar))
		mHPBar->SetMaxStatText(_StatVal);
}

void UInGameMainWidget::SetMaxStaminaText(float _StatVal)
{
	if (IsValid(mHPBar))
		mStaminaBar->SetMaxStatText(_StatVal);
}
