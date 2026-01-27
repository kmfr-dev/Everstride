// Fill out your copyright notice in the Description page of Project Settings.


#include "FireRock.h"
#include "../Actors/CharacterBase.h"
#include "../Component/SkillComponent.h"
#include "../Skill/ProjectileSkill.h"

void UFireRock::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	ACharacterBase* OwnerPtr = MeshComp->GetOwner<ACharacterBase>();
	if (nullptr == OwnerPtr)
		return;

	USkillComponent* SkillComp = OwnerPtr->GetSkillComponent();
	if (nullptr == SkillComp)
		return;

	AProjectileSkill* CastingSkill = Cast< AProjectileSkill>(SkillComp->GetCastingSkillActor());
	if (nullptr == CastingSkill)
		return;

	CastingSkill->Fire();
}