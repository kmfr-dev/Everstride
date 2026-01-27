// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "Roar.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API ARoar : public ASkillBase
{
	GENERATED_BODY()
	
public:
	ARoar();

protected:
	UPROPERTY(EditAnywhere)
	float mRoarRadius = 0.f;

protected:
	virtual void BeginPlay() override;

public:
	virtual bool ExecuteSkill(ACharacter* _Owner, const FName& _ID);

public:
	UFUNCTION()
	void Roar();
};