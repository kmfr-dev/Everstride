// Fill out your copyright notice in the Description page of Project Settings.


#include "ThrowRockSkill.h"
#include "Components/CapsuleComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "../Component/StatComponent.h"
#include "../Actors/Monster/Controller/AIControllerMonster.h"
#include "../Subsystem/TableSubsystem.h"

AThrowRockSkill::AThrowRockSkill()
{
	mCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Collision"));
	mCollision->SetEnableGravity(false);
	mCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel6);

	RootComponent = mCollision;

	mMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mMesh->SetupAttachment(RootComponent);
}

void AThrowRockSkill::BeginPlay()
{
	Super::BeginPlay();

	mCollision->OnComponentBeginOverlap.AddDynamic(this, &AThrowRockSkill::OnCollisionBeginOverlap);
}

bool AThrowRockSkill::ExecuteSkill(ACharacter* _Owner, const FName& _ID)
{
	Super::ExecuteSkill(_Owner, _ID);

	SetActorRotation(mSkillCaster->GetActorRotation());
	AttachToComponent(mSkillCaster->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("FX_Trail_R_01"));


	return true;
}

void AThrowRockSkill::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (mHitActors.Contains(OtherActor))
		return;

	ACharacterBase* SkillCaster = Cast< ACharacterBase>(mSkillCaster);
	if (nullptr == SkillCaster)
		return;

	UStatComponent* StatComp =  SkillCaster->GetStatComponent();
	if (nullptr == StatComp)
		return;

	UTableSubsystem* TableSub = UTableSubsystem::Get(this);
	if (nullptr == TableSub)
		return;

	FSkillTableRow* SkillRow = TableSub->FindTableRow<FSkillTableRow>(UTableData::TableName::SKILL, mSkillID);
	if (nullptr == SkillRow)
		return;

	const float Attack = StatComp->GetTotalStatValue(EStatusType::STATTYPE_ATTACK);

	float Damage = Attack + SkillRow->Value;

	// 맞은 지점에 이펙트 처리
	const FVector& HitPoint = SweepResult.ImpactPoint;
	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		mHitEffect,
		HitPoint,
		FRotator::ZeroRotator
	);

	// 데미지 처리
	FDamageEvent DamageEvent;
	OtherActor->TakeDamage(Damage, DamageEvent, SkillCaster->GetController(), SkillCaster);

	mHitActors.Emplace(OtherActor);
}

void AThrowRockSkill::OnSpawnFromPool_Implementation()
{
	Super::OnSpawnFromPool_Implementation();
	mCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AThrowRockSkill::OnReturnToPool_Implementation()
{
	Super::OnReturnToPool_Implementation();

	mHitActors.Reset();
	SetActorLocation(FVector::ZeroVector);
	
	mCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AThrowRockSkill::Fire()
{
	AAIControllerMonster* Controller = mSkillCaster->GetController<AAIControllerMonster>();
	if (nullptr == Controller)
		return;

	AActor* Target = Controller->GetTarget();
	if (nullptr == Target)
		return;

	mCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	mTargetLocation = Target->GetActorLocation();

	// 해당 소켓의 위치를 얻어온다.
	FVector FireStart = mSkillCaster->GetMesh()->GetSocketLocation("FX_Trail_R_01");

	// 투사체 방향 결정, 단순히 방향을 스킬시전자의 FowardVector로 하게되면 소켓기준으로 날라가게됨.
	FVector Dir = (mTargetLocation - FireStart).GetSafeNormal();

	// 소켓에서 Detach
	DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	// 투사체 속도벡터 지정
	mProjectileMovement->Velocity = Dir * mProjectileMovement->InitialSpeed;

	UTableSubsystem* TableSub = UTableSubsystem::Get(this);
	if (nullptr == TableSub)
		return;

	FSkillTableRow* SkillRow = TableSub->FindTableRow<FSkillTableRow>(UTableData::TableName::SKILL, mSkillID);
	if (nullptr == SkillRow)
		return;

	// 라이프 타임 계산
	const float LifeTime = SkillRow->MaxRange / mProjectileMovement->InitialSpeed;

	FTimerHandle SkillTimer;
	GetWorld()->GetTimerManager().SetTimer(mSpinTimer, this, &AThrowRockSkill::Spin, 0.01f, true);
	GetWorld()->GetTimerManager().SetTimer(SkillTimer, this, &AThrowRockSkill::FireEnd, LifeTime, false);
	
}

void AThrowRockSkill::FireEnd()
{
	// 이펙트 처리
	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		mHitEffect,
		GetActorLocation(),
		FRotator::ZeroRotator
	);

	// 풀로 리턴
	EndSkill();

	GetWorld()->GetTimerManager().ClearTimer(mSpinTimer);
}

void AThrowRockSkill::Spin()
{
	const float DeltaTime = GetWorld()->GetDeltaSeconds();
	AddActorLocalRotation(FRotator(0.f, 0.f, SpinSpeed * DeltaTime));
}
