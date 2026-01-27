// Fill out your copyright notice in the Description page of Project Settings.


#include "AIControllerBossMonster.h"
#include "Perception/AIPerceptionComponent.h"
#include "BehaviorTree/BlackboardComponent.h"


AAIControllerBossMonster::AAIControllerBossMonster()
{
	bIsRunTree = false;
}

void AAIControllerBossMonster::SetBossMonsterHP(float _NewPercent)
{
	Blackboard->SetValueAsFloat(TEXT("HP"), _NewPercent);
}

void AAIControllerBossMonster::OnTargetFound(AActor* Actor, FAIStimulus Stimulus)
{
	// .. 
}