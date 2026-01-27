// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SkillBase.h"
#include "ProjectileSkill.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API AProjectileSkill : public ASkillBase
{
	GENERATED_BODY()

public:
	AProjectileSkill();

protected:
	// 충돌체
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UShapeComponent> mCollision = nullptr;

	// 투사체 무브먼트
	UPROPERTY(EditAnywhere)
	TObjectPtr<class UProjectileMovementComponent> mProjectileMovement = nullptr;
	

protected:
	UFUNCTION()
	virtual void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:
	virtual void OnSpawnFromPool_Implementation() override;
	virtual void OnReturnToPool_Implementation() override;

public:
	// 투사체 발사 함수
	UFUNCTION()
	virtual void Fire();

protected:
	// 투사체 발사 종료 함수
	UFUNCTION()
	virtual void FireEnd();
};
