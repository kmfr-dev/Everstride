// Fill out your copyright notice in the Description page of Project Settings.


#include "TurnOwnerNotify.h"
#include "../Actors/Player/PlayerBase.h"
#include "../Component/PlayerCombatComponent.h"

void UTurnOwnerNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	APlayerBase* OwnerPtr = MeshComp->GetOwner<APlayerBase>();
	if (nullptr == OwnerPtr)
		return;

	UPlayerCombatComponent* CombatComp = Cast<UPlayerCombatComponent>(OwnerPtr->GetCombatComponent());
	if (nullptr == CombatComp)
		return;

	CombatComp->TurnOwner();
}

