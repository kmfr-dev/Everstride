// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillSlot.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "../Common/UIView.h"

void USkillSlot::NativeConstruct()
{
	Super::NativeConstruct();

	CoolDownFormat.SetMaximumFractionalDigits(0);

	SetOpacity_CoolDownImage(0.f);
	SetOpacity_SkillImage(1.f);
	SetOpacity_CoolDownText(0.f);
}

void USkillSlot::NativeDestruct()
{
	Super::NativeDestruct();

	mSkillCoolDownHandle.Invalidate();
}

void USkillSlot::NativeOnListItemObjectSet(UObject* ListItemObject)
{
	USkillView* SkillItem = Cast<USkillView>(ListItemObject);
	if (true == IsValid(SkillItem))
	{
		mSkillImage->SetBrushFromTexture(SkillItem->GetSkillIcon());
	}
}

void USkillSlot::SetCoolDown(float _CoolDown)
{
	EnableCoolDown(true);
	SetCoolDownText(_CoolDown);
	mSkillCooldownElapsed = 0.0f;

	GetWorld()->GetTimerManager().SetTimer(
		mSkillCoolDownHandle,
		FTimerDelegate::CreateLambda([this, _CoolDown]() 
			{
				mSkillCooldownElapsed += 0.01f;

				SetCoolDownText(_CoolDown - mSkillCooldownElapsed);
				if (mSkillCooldownElapsed >= _CoolDown)
				{
					ClearCoolDown();
				}
			}),
		0.01f,
		true
	);
}

void USkillSlot::SetOpacity_CoolDownImage(float _NewOpacity)
{
	mCoolDownImage->SetOpacity(_NewOpacity);
}

void USkillSlot::SetOpacity_SkillImage(float _NewOpacity)
{
	mSkillImage->SetOpacity(_NewOpacity);
}

void USkillSlot::SetOpacity_CoolDownText(float _NewOpacity)
{
	mCoolDownText->SetOpacity(_NewOpacity);
}

void USkillSlot::SetCoolDownText(float _CoolDown)
{
	mCoolDownText->SetText(FText::AsNumber(_CoolDown, &CoolDownFormat));
}

void USkillSlot::ClearCoolDown()
{
	EnableCoolDown(false);
	GetWorld()->GetTimerManager().ClearTimer(mSkillCoolDownHandle);
}

void USkillSlot::EnableCoolDown(bool _Enable)
{
	float CoolDownImage_Opacity = 1.f;
	float SkillImage_Opacity = 0.5f;
	float CoolDownText_Opacity = 1.f;

	if(false == _Enable)
	{
		CoolDownImage_Opacity = 0.f;
		SkillImage_Opacity = 1.f;
		CoolDownText_Opacity = 0.f;
	}

	SetOpacity_CoolDownImage(CoolDownImage_Opacity);
	SetOpacity_SkillImage(SkillImage_Opacity);
	SetOpacity_CoolDownText(CoolDownText_Opacity);
}
