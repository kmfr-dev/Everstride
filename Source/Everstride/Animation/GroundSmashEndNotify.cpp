// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundSmashEndNotify.h"
#include "../Actors/CharacterBase.h"
#include "../Component/SkillComponent.h"
#include "../Skill/GroundSmash.h"

void UGroundSmashEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	ACharacterBase* OwnerPtr = MeshComp->GetOwner<ACharacterBase>();
	if (nullptr == OwnerPtr)
		return;

	USkillComponent* SkillComp = OwnerPtr->GetSkillComponent();
	if (nullptr == SkillComp)
		return;

	AGroundSmash* SkillActor = Cast< AGroundSmash>(SkillComp->GetCastingSkillActor());
	if (nullptr == SkillActor)
		return;

	SkillActor->SkillHitCheck();
}
