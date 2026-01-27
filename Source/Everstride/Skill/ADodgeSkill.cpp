// Fill out your copyright notice in the Description page of Project Settings.


#include "ADodgeSkill.h"
#include "../Actors/Player/PlayerBase.h"
#include "../Subsystem/TableSubsystem.h"
#include "../Component/SkillComponent.h"

bool AADodgeSkill::ExecuteSkill(ACharacter* _Owner, const FName& _ID)
{
	APlayerBase* PlayerPtr = Cast<APlayerBase>(_Owner);
	if (nullptr == PlayerPtr)
		return false;

	/*USkillComponent* SkillComp = PlayerPtr->GetSkillComponent();
	if (nullptr == SkillComp)
		return false;

	SkillComp->SkillEnd();
	*/
	Destroy();
	
	return true;
}
