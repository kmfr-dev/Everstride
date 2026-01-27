// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIControllerBase.h"
#include "GenericTeamAgentInterface.h"
#include "AIControllerMonster.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API AAIControllerMonster : public AAIControllerBase
{
	GENERATED_BODY()
	
public:
	AAIControllerMonster();

protected:
	UPROPERTY()
	bool bIsRunTree = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAIPerceptionComponent> AIPerception;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UAISenseConfig_Sight> SightConfig;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;
	
public:
	UFUNCTION()
	virtual void SetBlackBoardData();

	UFUNCTION()
	void SetTarget(AActor* _TargetActor);

	UFUNCTION()
	AActor* GetTarget();

protected:
	UFUNCTION()
	virtual void OnTargetFound(AActor* Actor, FAIStimulus Stimulus);
};
