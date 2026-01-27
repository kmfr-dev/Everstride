// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectileSkill.h"
#include "SlashSkill.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API ASlashSkill : public AProjectileSkill
{
	GENERATED_BODY()
	
public:
	ASlashSkill();

protected:
	// 적중 이펙트
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UNiagaraSystem> mHitEffect;
	// 검기 이펙트
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = true))
	TObjectPtr<class UNiagaraComponent> mEffect;

	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> mHitActors;

protected:
	virtual void BeginPlay() override;

protected:
	virtual bool ExecuteSkill(ACharacter* _Owner, const FName& _ID) override;
	virtual void OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void FireEnd() override;

public:
	virtual void OnSpawnFromPool_Implementation() override;
	virtual void OnReturnToPool_Implementation() override;
};
