// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossRoomTrigger.generated.h"

UCLASS()
class EVERSTRIDE_API ABossRoomTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABossRoomTrigger();

protected:
	// 레벨 시퀀스 에셋
	UPROPERTY(EditAnywhere)
	TObjectPtr<class ULevelSequence> mLevelSequenceAsset = nullptr;

	UPROPERTY(EditAnyWhere)
	TObjectPtr<class UBoxComponent> mCollision = nullptr;

	UPROPERTY()
	TObjectPtr<class APlayerBase> mPlayer = nullptr;
	
	UPROPERTY()
	TObjectPtr<class ABossMonsterBase> mBoss = nullptr;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:
	UFUNCTION()
	void OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSequenceStart();

	UFUNCTION()
	void OnSequenceEnd();

	UFUNCTION()
	void BossCombatReady();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
