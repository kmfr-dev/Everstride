// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interface/ObjectPoolInterface.h"
#include "Projectile.generated.h"

UCLASS()
class EVERSTRIDE_API AProjectile : public AActor, public IObjectPoolInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)

	TObjectPtr<class UProjectileMovementComponent> mMovement = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UBoxComponent> mCollision = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UParticleSystemComponent> mEffectComp = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<class UParticleSystem> mHitEffect = nullptr;
	
private:
	// 라이프타임 타이머
	FTimerHandle mLifetimeTimer;
	// 거리체크용 타이머
	FTimerHandle mDistanceTimer;
	// 라이프타임 계산용
	float	mMaxLifeTime = 0.f;
	float	mAttackRange = 0.f;
	FVector mStartLocation = FVector::ZeroVector;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION()
	virtual void OnSpawnFromPool_Implementation() override;

	UFUNCTION()
	virtual void OnReturnToPool_Implementation() override;

	UFUNCTION()
	void Fire(const FVector& _TargetLoc);

private:
	UFUNCTION()
	void CheckDistance();
	
	UFUNCTION()
	void OnLifeTimeExpired();

	UFUNCTION()
	void EnableEffect(bool _Enable);

	UFUNCTION()
	void ReturnPool();

	UFUNCTION()
	void MovementReset();
};
