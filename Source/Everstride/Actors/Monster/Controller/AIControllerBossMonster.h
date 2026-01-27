// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIControllerMonster.h"
#include "AIControllerBossMonster.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API AAIControllerBossMonster : public AAIControllerMonster
{
	GENERATED_BODY()
	
public:
	AAIControllerBossMonster();

public:
	UFUNCTION()
	void SetBossMonsterHP(float _NewPercent);

protected:
	virtual void OnTargetFound(AActor* Actor, FAIStimulus Stimulus) override;
};
