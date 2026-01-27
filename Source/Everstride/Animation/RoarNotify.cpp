// Fill out your copyright notice in the Description page of Project Settings.


#include "RoarNotify.h"
#include "../Actors/CharacterBase.h"
#include "../Component/SkillComponent.h"
#include "../Skill/Roar.h"

void URoarNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ACharacterBase* OwnerPtr = MeshComp->GetOwner<ACharacterBase>();
	if (nullptr == OwnerPtr)
		return;
	
	USkillComponent* SkillComp = OwnerPtr->GetSkillComponent();
	if (nullptr == SkillComp)
		return;

	ARoar* RoarSkill = Cast<ARoar>(SkillComp->GetCastingSkillActor());
	if (nullptr == RoarSkill)
		return;

	RoarSkill->Roar();
}
