// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Common/GameDefines.h"
#include "Animation/AnimInstance.h"
#include "AnimInstanceBase.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API UAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	// 나를 소유한 오너 캐릭터
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<class ACharacterBase> mOwnerCharacter = nullptr;

	// 몽타주(특수동작 구분)
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, TObjectPtr<UAnimMontage>> mMontage;

	// 기본 동작 Transition을 위한 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float mSpeed = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float mJumpThreshold = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool mIsJumping = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool mIsFalling = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool mIsLand = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool mIsHit = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool mIsDead = false;

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
	virtual void NativeUninitializeAnimation() override;

public:
	UFUNCTION()
	void SetAnimData(const struct FAnimationTableRow& _Row);

	UFUNCTION()
	UAnimMontage* GetAnimMontage(const FGameplayTag& _Tag) const;

	UFUNCTION(BlueprintCallable)
	void PlayAnimMontage(const FGameplayTag& _Tag);


private:
	UFUNCTION()
	void InitTransitionData();

	UFUNCTION()
	void UpdateMovement();
};
