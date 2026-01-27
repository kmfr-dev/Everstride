// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillBase.h"
#include "ADodgeSkill.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API AADodgeSkill : public ASkillBase
{
	GENERATED_BODY()
	
public:
	virtual bool ExecuteSkill(ACharacter* _Owner, const FName& _ID);
};
