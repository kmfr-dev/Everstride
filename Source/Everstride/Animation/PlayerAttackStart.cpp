// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAttackStart.h"
#include "../Interface/AttackInterface.h"

void UPlayerAttackStart::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);


	IAttackInterface* OwnerPtr = Cast<IAttackInterface>(MeshComp->GetOwner());
	if (nullptr == OwnerPtr)
		return;

	OwnerPtr->AttackHitCheck();
}
