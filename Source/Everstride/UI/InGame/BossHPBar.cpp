// Fill out your copyright notice in the Description page of Project Settings.


#include "BossHPBar.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"

void UBossHPBar::SetBossName(const FString& _BossName)
{
	mBossName->SetText(FText::FromString(_BossName));
}

void UBossHPBar::SetHPPercent(float _NewPercent)
{
	mHPBar->SetPercent(_NewPercent);
}

void UBossHPBar::SetHPText(const FString& _Text)
{
	mHPText->SetText(FText::FromString(_Text));
}
