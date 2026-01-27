// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AIControllerBase.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API AAIControllerBase : public AAIController
{
	GENERATED_BODY()
	
public:
	AAIControllerBase();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UBehaviorTree> mAITree = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UBlackboardData> mBoardData = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	uint8 mTeamID;

public:
	void RunTree();
	void StopBehaviorTree();

public:
	virtual void SetGenericTeamId(const FGenericTeamId& _TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;
};
