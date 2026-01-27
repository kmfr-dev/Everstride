// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PlayerInputSystem.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API UPlayerInputSystem : public UObject
{
	GENERATED_BODY()
	
public:
	UPlayerInputSystem();

private:
	// 매핑 컨텍스트
	UPROPERTY()
	TObjectPtr<class UInputMappingContext> mMappingContext;

	// 이동 액션
	UPROPERTY()
	TObjectPtr<class UInputAction> mMoveAction;

	// 카메라 회전 액션
	UPROPERTY()
	TObjectPtr<class UInputAction> mCameraRotAction;

	// 달리기 액션
	UPROPERTY()
	TObjectPtr<class UInputAction> mSprintAction;

	// 점프 액션
	UPROPERTY()
	TObjectPtr<class UInputAction> mJumpAction;

	// 공격 액션
	UPROPERTY()
	TObjectPtr<class UInputAction> mAttackAction;

	UPROPERTY()
	TObjectPtr<class UInputAction> mSlashSkillAction;

	UPROPERTY()
	TObjectPtr<class UInputAction> mDodgeSkillAction;

public:
	UFUNCTION()
	class UInputMappingContext* GetContext();
	
	UFUNCTION()
	class UInputAction*			GetMoveAction();
	
	UFUNCTION()
	class UInputAction*			GetCameraRotAction();

	UFUNCTION()
	class UInputAction*			GetSprintAction();

	UFUNCTION()
	class UInputAction*			GetAttackAction();

	UFUNCTION()
	class UInputAction*			GetJumpAction();
	
	UFUNCTION()
	class UInputAction*			GetSlashSkillAction();	
	
	UFUNCTION()
	class UInputAction*			GetDodgeSkillAction();
};
