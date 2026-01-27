// Fill out your copyright notice in the Description page of Project Settings.


#include "RangeNormalAttack.h"
#include "../Subsystem/ObjectPoolSubsystem.h"
#include "Projectile.h"

bool URangeNormalAttack::ExecuteAttack(ACharacter* _Owner)
{
	if (nullptr == _Owner || nullptr == mProjectileClass)
		return false;
		
	UObjectPoolSubsystem* PoolSub = UObjectPoolSubsystem::Get(this);
	if (nullptr == PoolSub)
		return false;

	bool IsPooled = PoolSub->IsPooled(mProjectileClass);
	if (false == IsPooled)
		PoolSub->InitPool(mProjectileClass, POOLSIZE);

	AProjectile* Projectile = Cast<AProjectile>(PoolSub->GetActorFromPool(mProjectileClass));
	if (nullptr == Projectile)
		return false;
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(_Owner, 0);
	if (nullptr == PlayerPawn)
		return false;

	Projectile->SetOwner(_Owner); 
	const FVector& MuzzleLoc = _Owner->GetMesh()->GetSocketLocation(TEXT("Muzzle_Front"));
	Projectile->SetActorTransform(FTransform(MuzzleLoc));
	Projectile->Fire(PlayerPawn->GetActorLocation());

	return true;
}
