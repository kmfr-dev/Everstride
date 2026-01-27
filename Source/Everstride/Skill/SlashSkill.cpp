// Fill out your copyright notice in the Description page of Project Settings.


#include "SlashSkill.h"
#include "NiagaraComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

#include "Components/BoxComponent.h"
#include "../Component/SkillComponent.h"
#include "../Component/StatComponent.h"
#include "../Actors/Player/PlayerBase.h"
#include "../Subsystem/TableSubsystem.h"

ASlashSkill::ASlashSkill()
{
	mCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	mCollision->SetEnableGravity(false);
	mCollision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel5);

	RootComponent = mCollision;

	mEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("SlashEffect"));
	mEffect->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform, FName("NiagaraComponent"));
}

void ASlashSkill::BeginPlay()
{
	Super::BeginPlay();

	if (nullptr == mCollision)
		return;

	mCollision->OnComponentBeginOverlap.AddDynamic(this, &ASlashSkill::OnCollisionBeginOverlap);
}

bool ASlashSkill::ExecuteSkill(ACharacter* _Owner, const FName& _ID)
{
	Super::ExecuteSkill(_Owner, _ID);

	// 초기위치, 회전값 설정
	SetActorLocation(_Owner->GetActorLocation());
	SetActorRotation(FRotator(_Owner->GetActorRotation().Pitch, _Owner->GetActorRotation().Yaw, -45.f));

	mProjectileMovement->Velocity = _Owner->GetActorForwardVector() * mProjectileMovement->InitialSpeed;

	UTableSubsystem* TableSub = UTableSubsystem::Get(this);
	if (nullptr == TableSub)
		return false;

	FSkillTableRow* SkillRow = TableSub->FindTableRow<FSkillTableRow>(UTableData::TableName::SKILL, _ID);
	if (nullptr == SkillRow)
		return false;
	
	mEffect->Activate(true);

	// 라이프 타임 계산
	const float LifeTime = SkillRow->MaxRange / mProjectileMovement->InitialSpeed;

	FTimerHandle SkillTimer;
	GetWorld()->GetTimerManager().SetTimer(SkillTimer, this, &ASlashSkill::FireEnd, LifeTime, false);

	return true;
}

void ASlashSkill::FireEnd()
{

	Super::FireEnd();
}

void ASlashSkill::OnCollisionBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 이미 전에 적이 맞았다면 return
	if (mHitActors.Contains(OtherActor))
		return;
	
	ACharacterBase* Player = Cast<ACharacterBase>(mSkillCaster);
	if (nullptr == Player)
		return;

	UStatComponent* StatComp = Player->GetStatComponent();
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

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		GetWorld(), 
		mHitEffect, 
		HitPoint, 
		FRotator::ZeroRotator,
		FVector(1.f), 
		true, 
		true, 
		ENCPoolMethod::AutoRelease
	);


	// 데미지 처리
	FDamageEvent DamageEvent;
	OtherActor->TakeDamage(Damage, DamageEvent, Player->GetController(), Player);

	mHitActors.Emplace(OtherActor);

}

void ASlashSkill::OnSpawnFromPool_Implementation()
{
	Super::OnSpawnFromPool_Implementation();
}

void ASlashSkill::OnReturnToPool_Implementation()
{
	Super::OnReturnToPool_Implementation();

	mEffect->Deactivate();

	mHitActors.Reset();
}
