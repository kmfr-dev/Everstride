// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCombatComponent.h"
#include "../DataAsset/ComboActionData.h"
#include "../Animation/AnimInstanceBase.h"
#include "../Actors/Player/PlayerBase.h"
#include "NiagaraComponent.h"

UPlayerCombatComponent::UPlayerCombatComponent()
{
	static ConstructorHelpers::FObjectFinder<UComboActionData>
		ComboActionDataRef(TEXT("/Script/Everstride.ComboActionData'/Game/DataAsset/DA_ComboActionData.DA_ComboActionData'"));
	if (ComboActionDataRef.Succeeded())
		mComboData = ComboActionDataRef.Object;
}

void UPlayerCombatComponent::AttackBegin()
{
	ProcessComboAttack();
}

void UPlayerCombatComponent::AttackHitCheck()
{
	Super::AttackHitCheck();
}

void UPlayerCombatComponent::ProcessComboAttack()
{
	if (mCurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	if (!mComboTimerHandle.IsValid())
	{
		mHasNextComboCommand = false;
	}
	else
	{
		mHasNextComboCommand = true;
	}
}

void UPlayerCombatComponent::ComboActionBegin()
{
	ACharacter* OwnerPtr = GetOwner<ACharacter>();
	if (nullptr == OwnerPtr)
		return;


	mCurrentCombo = 1;
	mIsCombat = true;

	UAnimInstanceBase* AnimInstancePtr = Cast< UAnimInstanceBase>(OwnerPtr->GetMesh()->GetAnimInstance());
	if (nullptr == AnimInstancePtr)
		return;

	UAnimMontage* AttackMontage = AnimInstancePtr->GetAnimMontage(EverstrideGamePlayTags::TAG_AnimMontage_Player_Attack);
	if (nullptr == AttackMontage)
		return;

	AnimInstancePtr->Montage_Play(AttackMontage, 1.f);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &UPlayerCombatComponent::ComboActionEnd);
	AnimInstancePtr->Montage_SetEndDelegate(EndDelegate);

	mComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void UPlayerCombatComponent::ComboActionEnd(UAnimMontage* _TargetMontage, bool _IsProperyEnded)
{
	ensure(mCurrentCombo != 0);
	mCurrentCombo = 0;

	mIsCombat = false;

	GetWorld()->GetTimerManager().ClearTimer(mComboTimerHandle);
	mComboTimerHandle.Invalidate();

}

void UPlayerCombatComponent::SetComboCheckTimer()
{
	int32 ComboIndex = mCurrentCombo - 1;
	ensure(mComboData->EffectiveFrameCount.IsValidIndex(ComboIndex));

	const float AttackSpeedRate = 1.0f;
	float ComboEffectiveTime = (mComboData->EffectiveFrameCount[ComboIndex] / mComboData->FrameRate) / AttackSpeedRate;
	if (ComboEffectiveTime > 0.0f)
	{
		GetWorld()->GetTimerManager().SetTimer(mComboTimerHandle, this, &UPlayerCombatComponent::ComboCheck, ComboEffectiveTime, false);
	}
}

void UPlayerCombatComponent::ComboCheck()
{
	ACharacter* OwnerPtr = GetOwner<ACharacter>();
	if (nullptr == OwnerPtr)
		return;

	mComboTimerHandle.Invalidate();
	if (mHasNextComboCommand)
	{
		UAnimInstanceBase* AnimInstancePtr = Cast< UAnimInstanceBase>(OwnerPtr->GetMesh()->GetAnimInstance());
		if (nullptr == AnimInstancePtr)
			return;

		UAnimMontage* AttackMontage = AnimInstancePtr->GetAnimMontage(EverstrideGamePlayTags::TAG_AnimMontage_Player_Attack);
		if (nullptr == AttackMontage)
			return;
		

		mCurrentCombo = FMath::Clamp(mCurrentCombo + 1, 1, mComboData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *mComboData->MontageSectionName, mCurrentCombo);
		AnimInstancePtr->Montage_JumpToSection(NextSection, AttackMontage);
		mHasNextComboCommand = false;
		SetComboCheckTimer();
	}
}

void UPlayerCombatComponent::TurnOwner()
{
	ACharacter* OwnerPtr = GetOwner<ACharacter>();
	if (nullptr == OwnerPtr)
		return;

	
	OwnerPtr->SetActorRotation(FRotator(0.f, OwnerPtr->GetControlRotation().Yaw, 0.f));
}
