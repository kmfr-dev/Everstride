// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Common/GameDefines.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "UISubsystem.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API UUISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	// 현재 표시할 메인 위젯
	UPROPERTY()
	TObjectPtr<UUserWidget> mCurrentMainWidget = nullptr;

public:
	// 기본 메인위젯 세팅
	UFUNCTION()
	void SetMainWidget();

public:
	// 메인 위젯 제거 함수
	UFUNCTION()
	void ClearMainWidget();

public:
	static UUISubsystem* Get(const UObject* _WorldContext);
};
