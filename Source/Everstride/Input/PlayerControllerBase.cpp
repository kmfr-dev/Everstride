// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllerBase.h"
#include "../Subsystem/UISubsystem.h"

void APlayerControllerBase::BeginPlay()
{
	Super::BeginPlay();

	// 메인 위젯 세팅
	UUISubsystem* UISub = UUISubsystem::Get(this);
	if (UISub)
		UISub->SetMainWidget();
}
