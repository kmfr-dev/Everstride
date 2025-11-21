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

public:
	class UInputMappingContext* GetContext();
	class UInputAction* GetMoveAction();
	class UInputAction* GetCameraRotAction();
};
