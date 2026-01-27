// Fill out your copyright notice in the Description page of Project Settings.


#include "UseSkillNotify.h"
#include "../Actors/Player/PlayerBase.h"
#include "../Component/SkillComponent.h"

void UUseSkillNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	APlayerBase* PlayerPtr = MeshComp->GetOwner<APlayerBase>();
	if (nullptr == PlayerPtr)
		return;

	USkillComponent* SkillComp = PlayerPtr->GetSkillComponent();
	if(nullptr == SkillComp)
		return;
	
	SkillComp->SkillActionStart();
}
