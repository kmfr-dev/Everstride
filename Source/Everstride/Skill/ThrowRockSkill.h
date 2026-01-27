// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileSkill.h"
#include "ThrowRockSkill.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API AThrowRockSkill : public AProjectileSkill
{
	GENERATED_BODY()
public:
	AThrowRockSkill();

protected:
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UParticleSystem> mHitEffect = nullptr;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> mMesh;

	UPROPERTY(EditAnywhere)
	float SpinSpeed = 0.f;

	UPROPERTY()
	FVector mTargetLocation = FVector::ZeroVector;

	FTimerHandle mSpinTimer;

	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> mHitActors;
protected:
	virtual void BeginPlay() override;
	virtual bool ExecuteSkill(ACharacter* _Owner, const FName& _ID) override;
	virtual void Fire() override;
	virtual void FireEnd() override;

	void Spin();

protected:
	virtual void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

public:
	virtual void OnSpawnFromPool_Implementation() override;
	virtual void OnReturnToPool_Implementation() override;

	UFUNCTION()
	void SetTargetLocation(const FVector& _Locatioon) { mTargetLocation = _Locatioon; }
};
