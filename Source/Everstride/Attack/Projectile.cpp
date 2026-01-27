// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "../Subsystem/ObjectPoolSubsystem.h"
#include "../Component/StatComponent.h"
#include "../Actors/CharacterBase.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mCollision = CreateDefaultSubobject<UBoxComponent>("Collision");
	mCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mCollision->SetCollisionObjectType(ECC_GameTraceChannel6);
	mCollision->SetGenerateOverlapEvents(true);
	RootComponent = mCollision;

	mEffectComp = CreateDefaultSubobject<UParticleSystemComponent>("Effect");
	mEffectComp->SetupAttachment(RootComponent);
	mEffectComp->SetAutoActivate(false);

	mMovement = CreateDefaultSubobject<UProjectileMovementComponent>("Movement");
	mMovement->bAutoActivate = false;
	mMovement->ProjectileGravityScale = 1.f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();

	mCollision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnProjectileBeginOverlap);
}

void AProjectile::OnProjectileBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ACharacterBase* OwnerPtr = GetOwner<ACharacterBase>();
	if (nullptr == OwnerPtr)
		return;

	UStatComponent* StatComp = OwnerPtr->GetStatComponent();
	if (nullptr == StatComp)
		return;

	// 이펙트 출력
	FVector HitPoint;
	OtherComp->GetClosestPointOnCollision(GetActorLocation(), HitPoint);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), mHitEffect, HitPoint);


	// 데미지를 준다
	const float Attack = StatComp->GetTotalStatValue(EStatusType::STATTYPE_ATTACK);

	FDamageEvent DamageEvent;
	OtherActor->TakeDamage(Attack, DamageEvent, OwnerPtr->GetController(), OwnerPtr);
	
	ReturnPool();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnSpawnFromPool_Implementation()
{
	// 무브먼트 활성화
	mMovement->Activate(true);

	// 이펙트도 켜줌
	EnableEffect(true);
}

void AProjectile::OnReturnToPool_Implementation()
{
	SetOwner(nullptr);

	mCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MovementReset();
	
	EnableEffect(false);

	if(mLifetimeTimer.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(mLifetimeTimer);
	
	if (mDistanceTimer.IsValid())
		GetWorld()->GetTimerManager().ClearTimer(mDistanceTimer);
}

void AProjectile::Fire(const FVector& _TargetLoc)
{
	ACharacterBase* OwnerPtr = GetOwner<ACharacterBase>();
	if (nullptr == OwnerPtr)
		return;

	UStatComponent* StatComp = OwnerPtr->GetStatComponent();
	if (nullptr == StatComp)
		return;

	const FVector& Start = GetActorLocation();
	mStartLocation = Start;

	FVector LanchVelocity = FVector::ZeroVector;
	const float LanchSpeed = mMovement->InitialSpeed;

	bool bResult = UGameplayStatics::SuggestProjectileVelocity(
		this,
		LanchVelocity,
		Start,
		_TargetLoc,
		LanchSpeed,
		false,
		0.f,
		0.f,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);

	if (!bResult)
		return;

	mMovement->ProjectileGravityScale = 1.f;
	// 구해준 속도벡터를 무브먼트에 설정후 활성화
	mMovement->Velocity = LanchVelocity;
	mMovement->Activate();

	// 해당 공격을 시전한 액터의 공격 범위를 얻어옴
	mAttackRange = StatComp->GetTotalStatValue(EStatusType::STATTYPE_ATTACKRANGE);

	FPredictProjectilePathParams Params;
	Params.StartLocation = Start;
	Params.LaunchVelocity = LanchVelocity;
	Params.bTraceWithCollision = false;
	Params.ProjectileRadius = 0.f;
	Params.MaxSimTime = 10.f;
	Params.SimFrequency = 15.f;

	// 월드 중력 * 발사체 중력 스케일
	const float GravityZ = GetWorld()->GetGravityZ() * mMovement->ProjectileGravityScale;

	Params.OverrideGravityZ = GravityZ;

	FPredictProjectilePathResult Result;
	UGameplayStatics::PredictProjectilePath(
		this,
		Params,
		Result
	);

	// 비행 종료 시간
	float LifeTime = Result.LastTraceDestination.Time;

	// 콜리전을 켜준다.
	mCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// 계산한 라이프타임 타이머
	GetWorld()->GetTimerManager().SetTimer(mLifetimeTimer, this, &AProjectile::OnLifeTimeExpired, LifeTime, false);

	// 거리체크 타이머
	GetWorld()->GetTimerManager().SetTimer(mDistanceTimer, this, &AProjectile::CheckDistance, 0.01f, true);
}

void AProjectile::CheckDistance()
{
	if (FVector::Dist(mStartLocation, GetActorLocation()) >= mAttackRange)
		ReturnPool();
}

void AProjectile::OnLifeTimeExpired()
{
	ReturnPool();
}

void AProjectile::EnableEffect(bool _Enable)
{
	if (false == IsValid(mEffectComp))
		return;

	if (_Enable)
		mEffectComp->Activate();

	else
		mEffectComp->Deactivate();
}

void AProjectile::ReturnPool()
{
	UObjectPoolSubsystem* PoolSub = UObjectPoolSubsystem::Get(this);
	if (nullptr == PoolSub)
		return;

	PoolSub->ReturnToPool(this);
}

void AProjectile::MovementReset()
{
	if (!mMovement)
		return;

	mMovement->StopMovementImmediately(); 
	mMovement->Deactivate();
	mMovement->Velocity = FVector::ZeroVector;
	mMovement->ProjectileGravityScale = 0.f;
}

