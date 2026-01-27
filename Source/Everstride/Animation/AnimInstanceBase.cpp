// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstanceBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Actors/CharacterBase.h"
#include "../Subsystem/TableSubsystem.h"
#include "Animation/AnimMontage.h"


void UAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();


	mOwnerCharacter = Cast<ACharacterBase>(TryGetPawnOwner());
}

void UAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	UpdateMovement();

	if(IsValid(mOwnerCharacter))
		mIsDead = mOwnerCharacter->IsDead();
}

void UAnimInstanceBase::NativeUninitializeAnimation()
{
	Super::NativeUninitializeAnimation();
}

void UAnimInstanceBase::SetAnimData(const FAnimationTableRow& _Row)
{
	mMontage = _Row.MontageMap;
}

UAnimMontage* UAnimInstanceBase::GetAnimMontage(const FGameplayTag& _Tag) const
{
	if (mMontage.Contains(_Tag))
	{
		return mMontage[_Tag];
	}

	return nullptr;
}

void UAnimInstanceBase::PlayAnimMontage(const FGameplayTag& _Tag)
{
	if (mMontage.Contains(_Tag))
	{
		Montage_Play(mMontage[_Tag]);
	}
}

void UAnimInstanceBase::InitTransitionData()
{
	UTableSubsystem* TableSub = UTableSubsystem::Get(this);
	if (nullptr == TableSub)
		return;

	FDefineTableRow* DefineRow = TableSub->FindTableRow<FDefineTableRow>(UTableData::TableName::DEFINE, UTableData::DefineName::JUMPTHRESHOLD);
	if (nullptr == DefineRow)
		return;

	mJumpThreshold = DefineRow->FloatVal;
}

void UAnimInstanceBase::UpdateMovement()
{
	if (IsValid(mOwnerCharacter))
	{
		UCharacterMovementComponent* MoveComp = mOwnerCharacter->GetCharacterMovement();
		if (IsValid(MoveComp))
		{
			FVector Velocity = MoveComp->Velocity;
			mSpeed = Velocity.Size2D();
			mIsFalling = MoveComp->IsFalling();
			mIsJumping = mIsFalling && Velocity.Z > mJumpThreshold;
		}
	}
}