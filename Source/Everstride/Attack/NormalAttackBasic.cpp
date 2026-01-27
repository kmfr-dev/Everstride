// Fill out your copyright notice in the Description page of Project Settings.


#include "NormalAttackBasic.h"
#include "../Component/StatComponent.h"
#include "../Actors/CharacterBase.h"

bool UNormalAttackBasic::ExecuteAttack(ACharacter* _Owner)
{
	// 나의 오너를 얻어온다.
	ACharacterBase* OwnerActor = Cast<ACharacterBase>(_Owner);
	if (nullptr == OwnerActor)
		return false;

	// 오너의 충돌체를 얻어온다.
	UCapsuleComponent* OwnerCollision = OwnerActor->GetCapsuleComponent();
	if (nullptr == OwnerCollision)
		return false;

	// 능력치 컴포넌트를 얻어온다.
	UStatComponent* StatComp = OwnerActor->GetStatComponent();
	if (nullptr == StatComp)
		return false;

	FCollisionQueryParams CollisionParams = FCollisionQueryParams(SCENE_QUERY_STAT(Attack), false, OwnerActor);
	CollisionParams.AddIgnoredComponent(OwnerActor->GetCapsuleComponent());

	// 공격 거리
	const float AttackRange = StatComp->GetTotalStatValue(EStatusType::STATTYPE_ATTACKRANGE);
	// 공격 범위
	const float AttackRadius = StatComp->GetTotalStatValue(EStatusType::STATTYPE_ATTACKRADIUS);
	// 데미지
	const float AttackDamage = StatComp->GetTotalStatValue(EStatusType::STATTYPE_ATTACK);

	// 캐릭터의 전방 벡터 및 캐릭터 위치를 얻어옴
	const FVector OwnerForward = OwnerActor->GetActorForwardVector();
	const FVector OwnerLocation = OwnerActor->GetActorLocation();

	// 시작 및 끝 지점 계산
	const FVector Start = OwnerLocation + OwnerForward * OwnerCollision->GetScaledCapsuleRadius();
	const FVector End = Start + OwnerForward * AttackRange;

	TArray<FHitResult> HitResult;

	bool HitDetect = GetWorld()->SweepMultiByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(AttackRadius),
		CollisionParams
	);

	if (HitDetect)
	{
		for (const FHitResult& Hit : HitResult)
		{
			AActor* HitActor = Hit.GetActor();
			// 만약 히트된 액터가 없거나 오너액터랑 동일한 액터면 continue;
			if (nullptr == HitActor || HitActor == OwnerActor)
				continue;

			FDamageEvent DamageEvent;
			HitActor->TakeDamage(AttackDamage, DamageEvent, OwnerActor->GetController(), OwnerActor);
		}
	}

	// 디버그 시각화
//#if ENABLE_DRAW_DEBUG && WITH_EDITOR
//	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
//	float CapsuleHalfHeight = AttackRange * 0.5f;
//	FColor DrawColor = HitDetect ? FColor::Red : FColor::Green;
//	DrawDebugCapsule(
//		GetWorld(),
//		CapsuleOrigin,
//		CapsuleHalfHeight + AttackRadius,
//		AttackRadius,
//		FRotationMatrix::MakeFromZ(OwnerForward).ToQuat(),
//		DrawColor,
//		false,
//		3.f);
//#endif


	return true;
}
