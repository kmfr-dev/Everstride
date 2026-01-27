// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMainWidget.h"
#include "StatBarWidget.h"
#include "BossHPBar.h"
#include "SkillSlot.h"
#include "Components/TileView.h"
#include "../Common/UIView.h"
#include "../../Subsystem/TableSubsystem.h"
#include "../../Actors/Player/InGamePlayerState.h"

void UInGameMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(mHPBar))
		mHPBar->SetStatBarColor(EStatBarType::STATBAR_TYPE_HP);

	if (IsValid(mStaminaBar))
		mStaminaBar->SetStatBarColor(EStatBarType::STATBAR_TYPE_STAMINA);
}

void UInGameMainWidget::SetSkillSlotUI(const FName& _PlayerTID)
{
	UTableSubsystem* TableSub = UTableSubsystem::Get(this);
	if (nullptr == TableSub)
		return;

	FPlayerTableRow* PlayerRow = TableSub->FindTableRow<FPlayerTableRow>(UTableData::TableName::PLAYER, _PlayerTID);
	if (nullptr == PlayerRow)
		return;

	// 플레이어 테이블의 스킬 TID 배열을 얻어와 반복,
	for (const FName& SkillTID : PlayerRow->SkillTIDs)
	{
		// 스킬 Table 조회
		FSkillTableRow* SkillRow = TableSub->FindTableRow<FSkillTableRow>(UTableData::TableName::SKILL, SkillTID);
		if (nullptr == SkillRow)
			continue;

		// 스킬 Table에 있는 TextureTID로 TextureTable을 조회
		FTextureTableRow* TextureRow = TableSub->FindTableRow<FTextureTableRow>(UTableData::TableName::TEXTURE, SkillRow->TextureTID);
		if (nullptr == TextureRow)
			continue;

		// 뷰 객체 생성
		USkillView* SkillView = NewObject<USkillView>(this);
		// 스킬 TID 설정
		SkillView->SetTID(SkillTID);
		// 스킬 Texture 설정
		SkillView->SetSkillIcon(TextureRow->Texture);
		// 최종적으로 타일뷰에 추가
		mSkillList->AddItem(SkillView);
	}
}

void UInGameMainWidget::UpdateSkillSlotUI(const FName& _SkillID)
{
	UTableSubsystem* TableSub = UTableSubsystem::Get(this);
	if (nullptr == TableSub)
		return;

	FSkillTableRow* SkillRow = TableSub->FindTableRow<FSkillTableRow>(UTableData::TableName::SKILL, _SkillID);
	if (nullptr == SkillRow)
		return;

	for (UObject* Item : mSkillList->GetListItems())
	{
		USkillView* SkillItem = Cast<USkillView>(Item);
		if (nullptr == SkillItem)
			continue;

		// 현재 아이템의 ID와 사용한 스킬 ID를 비교,
		// 같을 시 UI 업데이트 함수 호출
		if (SkillItem->GetTID() == _SkillID)
			SetSkillCoolDownUI(SkillItem, SkillRow->CoolDown);
	}
}

void UInGameMainWidget::SetEnableBossUI(bool _Enable)
{
	if (_Enable)
		mBossUI->SetVisibility(ESlateVisibility::HitTestInvisible);

	else
		mBossUI->SetVisibility(ESlateVisibility::Collapsed);
}

void UInGameMainWidget::SetBossName(const FString& _BossName)
{
	mBossUI->SetBossName(_BossName);
}

void UInGameMainWidget::SetBossHPBarPercent(float _NewPercent)
{
	mBossUI->SetHPText(FString::FormatAsNumber(_NewPercent * 100.f));
	mBossUI->SetHPPercent(_NewPercent);
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

void UInGameMainWidget::SetSkillCoolDownUI(USkillView* _Item, float _CoolDown)
{
	if (nullptr == _Item)
		return;

	USkillSlot* SkillSlotWidget = Cast<USkillSlot>(mSkillList->GetEntryWidgetFromItem(_Item));
	if (nullptr == SkillSlotWidget)
		return;

	SkillSlotWidget->SetCoolDown(_CoolDown);
}