// Fill out your copyright notice in the Description page of Project Settings.


#include "Roar.h"
#include "Components/SphereComponent.h"
#include "../Actors/Player/PlayerBase.h"
#include "../Component/StatComponent.h"
#include "../Subsystem/TableSubsystem.h"
#include "Engine/EngineTypes.h"

ARoar::ARoar()
{

}

void ARoar::BeginPlay()
{
	Super::BeginPlay();

}

bool ARoar::ExecuteSkill(ACharacter* _Owner, const FName& _ID)
{
	Super::ExecuteSkill(_Owner, _ID);


	return true;
}

void ARoar::Roar()
{
	if (nullptr == mSkillCaster)
		return;

    const float SkillDamage = GetSkillValue();

    const FVector& Center = mSkillCaster->GetActorLocation();

    TArray<FHitResult> HitResult;

    FCollisionQueryParams Params;
    Params.AddIgnoredActor(mSkillCaster);

    bool bHit = GetWorld()->SweepMultiByChannel(
        HitResult,
        Center,
        Center,
        FQuat::Identity,
		ECC_GameTraceChannel4,   
        FCollisionShape::MakeSphere(mRoarRadius),
        Params
    );

    for (const FHitResult& Result : HitResult)
    {
        AActor* HitActor = Result.GetActor();

        if (!HitActor) 
            continue;

		FDamageEvent DamageEvent;
		HitActor->TakeDamage(SkillDamage, DamageEvent, mSkillCaster->GetController(), mSkillCaster);
    }

  
	// 디버그 시각화
//#if ENABLE_DRAW_DEBUG
//	DrawDebugSphere(
//		GetWorld(),
//        Center,
//        mRoarRadius,
//		32,
//        bHit ? FColor::Red : FColor::Green,
//		false,
//		3.f
//	);
//#endif
}
