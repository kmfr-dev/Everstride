// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileSkill.h"
#include "GameFramework/ProjectileMovementComponent.h"

AProjectileSkill::AProjectileSkill()
{
	mProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	mProjectileMovement->bAutoActivate = false;
}

void AProjectileSkill::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Nothing
}

void AProjectileSkill::OnSpawnFromPool_Implementation()
{
	mProjectileMovement->SetActive(true);
	mProjectileMovement->Velocity = FVector::ZeroVector;
}

void AProjectileSkill::OnReturnToPool_Implementation()
{
	mProjectileMovement->SetActive(false);
}

void AProjectileSkill::Fire()
{
	// Nothing
}

void AProjectileSkill::FireEnd()
{
	EndSkill();
}
