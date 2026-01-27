// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Animation/AnimMontage.h"
#include "../Attack/AttackBase.h"
#include "../Subsystem/TableSubsystem.h"
#include "../Component/StatComponent.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
}


void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::AttackBegin()
{
	// Nothing.
}

void UCombatComponent::SetAttackData(TSubclassOf<UAttackBase> _Class)
{
	if (nullptr == _Class)
		return;

	mAttack = NewObject<UAttackBase>(this, _Class);
}

void UCombatComponent::AttackHitCheck()
{
	if (false == IsValid(mAttack))
		return;

	ACharacterBase* OwnerPtr = GetOwner<ACharacterBase>();
	if (nullptr == OwnerPtr)
		return;

	mAttack->ExecuteAttack(OwnerPtr);

	//// 거리가 가깝고 Forward Vector와 가장 가까운 대상 탐색
	//AActor* BestTarget = nullptr;
	//float	BestScore = -FLT_MAX;

	//// 가까운 적을 우선적으로 탐색
	//const float DotWeight = 10.f;       // 정면성 중요도
	//const float DistanceWeight = 100.f;      // 거리 중요도 

	//for (const FHitResult& Hit : HitResult)
	//{
	//	AActor* HitActor = Hit.GetActor();
	//	if (nullptr == HitActor)
	//		continue;

	//	const FVector ToTarget = (HitActor->GetActorLocation() - OwnerLocation);
	//	const float DistanceSquare = ToTarget.SizeSquared(); // 거리 제곱 사용
	//	const float Dot = FVector::DotProduct(OwnerForward, ToTarget.GetSafeNormal());

	//	// 점수 계산 
	//	// Dot 클수록 점수 증가 -> 정면에 가까울수록 우선
	//	// Distance가 작을수록 점수 증가 -> 가까울수록 우선
	//	float Score = Dot * DotWeight - DistanceSquare * DistanceWeight;

	//	if (Score > BestScore)
	//	{
	//		BestScore = Score;
	//		BestTarget = HitActor;
	//	}
	//}

	//if (BestTarget)
	//{
	//	// 데미지 처리

	//	UStatComponent* StatComp = OwnerActor->GetStatComponent();
	//	if (nullptr == StatComp)
	//		return;

	//	float TotalDamage = StatComp->GetTotalStatValue(EStatusType::STATTYPE_ATTACK);
	//	
	//	FDamageEvent DamageEvent;
	//	BestTarget->TakeDamage(TotalDamage, DamageEvent, OwnerActor->GetController(), OwnerActor);

	//	// TODO 추후 이펙트, 사운드 처리
	//}

}
