// Fill out your copyright notice in the Description page of Project Settings.


#include "GroundSmash.h"
#include "../Actors/Monster/BossMonsterBase.h"
#include "../Actors/Monster/Controller/AIControllerBossMonster.h"
#include "../Common/GameDefines.h"
#include "../Animation/AnimInstanceBase.h"
#include "../Subsystem/TableSubsystem.h"
#include "../Component/StatComponent.h"

void AGroundSmash::BeginPlay()
{
	Super::BeginPlay();
}

bool AGroundSmash::ExecuteSkill(ACharacter* _Owner, const FName& _ID)
{
	Super::ExecuteSkill(_Owner, _ID);

	ABossMonsterBase* BossMonster = Cast<ABossMonsterBase>(_Owner);
	if (nullptr == BossMonster)
		return false;

	AAIControllerBossMonster* ControllerPtr = BossMonster->GetController<AAIControllerBossMonster>();
	if (nullptr == ControllerPtr)
		return false;

	ACharacter* Target = Cast<ACharacter>(ControllerPtr->GetTarget());
	if (nullptr == Target)
		return false;

	UTableSubsystem* TableSub = UTableSubsystem::Get(this);
	if (nullptr == TableSub)
		return false;

	FSkillTableRow* SkillRow = TableSub->FindTableRow<FSkillTableRow>(UTableData::TableName::SKILL, _ID);
	if (nullptr == SkillRow)
		return false;

	const float SkillRange = SkillRow->MaxRange;
	const float TargetDist = BossMonster->GetDistanceTo(Target);

	// 스킬 범위보다 타겟 거리가 멀면 실패처리
	if (SkillRange < TargetDist)
		return false;

	// 스킬 시전자가 착지시 호출될 함수를 바인딩
	FOnLanded OnLandedFinished;
	OnLandedFinished.BindUObject(this, &AGroundSmash::GroundSmashEnd);
	BossMonster->SetLandedDelegate(OnLandedFinished);

	// 스킬 시전자의 위치와 Target의 위치를 얻어온다.
	const FVector& TargetLoc = Target->GetActorLocation();
	const FVector& OwnerLoc = BossMonster->GetActorLocation();
	
	FVector LanchVelocity = FVector::ZeroVector;

	// 시작 위치에서부터 목표 위치까지의 속도벡터를 구해준다.
	bool bResult = UGameplayStatics::SuggestProjectileVelocity(
		this,
		LanchVelocity,
		OwnerLoc,       
		TargetLoc,
		mLanchSpeed,
		false,
		0.f,
		mGravityZ,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);

	// 만약 해가 없다면 return
	if (false == bResult)
		return false;

	// MovementMode를 Falling상태로 변경 후
	// 구해준 속도벡터로 캐릭터를 밀어준다.
	BossMonster->GetCharacterMovement()->SetMovementMode(MOVE_Falling);
	BossMonster->LaunchCharacter(LanchVelocity, false, false);

	// 도약 시작하므로 Target과 스킬 시전자와의 충돌을 꺼준다.
	IgnoreCollisionToTarget(Target, true);

	return true;
}

void AGroundSmash::SkillHitCheck()
{
	if (nullptr == mSkillCaster)
		return;

	const float Damage = GetSkillValue();

	const FVector& Center = mSkillCaster->GetActorLocation();
	const FVector& Foward = mSkillCaster->GetActorForwardVector();
	const float CapsuleRadius = mSkillCaster->GetCapsuleComponent()->GetScaledCapsuleRadius();

	// 전방 오프셋
	const float FowardOffset = CapsuleRadius * 1.5f;
	const FVector SphereCenter = Center + Foward * FowardOffset;

	const FVector Start = SphereCenter + FVector(0, 0, 5.f);
	const FVector End = SphereCenter - FVector(0, 0, 5.f);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(mSkillCaster);

	TArray<FHitResult> HitResult;

	bool HitDetect = GetWorld()->SweepMultiByChannel(
		HitResult,
		Start,
		End,
		FQuat::Identity,
		ECC_GameTraceChannel4,
		FCollisionShape::MakeSphere(mHitRange),
		Params
	);

	if (HitDetect)
	{
		for (const FHitResult& Hit : HitResult)
		{
			AActor* HitActor = Hit.GetActor();
			if (!HitActor) 
				continue;

			FDamageEvent DamageEvent;
			HitActor->TakeDamage(Damage, DamageEvent, mSkillCaster->GetController(), mSkillCaster);
		}
	}

	// 디버그 시각화
//#if ENABLE_DRAW_DEBUG && WITH_EDITOR
//	DrawDebugSphere(
//		GetWorld(),
//		SphereCenter,
//		mHitRange,
//		24,
//		HitDetect ? FColor::Red : FColor::Green,
//		false,
//		3.f
//	);
//#endif
}

void AGroundSmash::GroundSmashEnd()
{
	if (nullptr == mSkillCaster)
		return;
	
	AAIControllerBossMonster* ControllerPtr = mSkillCaster->GetController<AAIControllerBossMonster>();
	if (nullptr == ControllerPtr)
		return;

	ACharacter* Target = Cast<ACharacter>(ControllerPtr->GetTarget());
	if (nullptr == Target)
		return;

	USkeletalMeshComponent* MeshPtr = mSkillCaster->GetMesh();
	if (nullptr == MeshPtr)
		return;
	
	UAnimInstanceBase* AnimInstancePtr = Cast<UAnimInstanceBase>(MeshPtr->GetAnimInstance());
	if (nullptr == AnimInstancePtr)
		return;

	UAnimMontage* GroundSMashMontage = AnimInstancePtr->GetAnimMontage(EverstrideGamePlayTags::TAG_Skill_BossMonster_GrondLand);
	if (nullptr == GroundSMashMontage)
		return;

	// 도약 종료 몽타주 섹션으로 섹션 점프
	AnimInstancePtr->Montage_JumpToSection(TEXT("End"), GroundSMashMontage);

	// 도약 종료 몽타주 재생 후 호출될 함수 바인딩
	FOnMontageEnded MontageEnded;
	MontageEnded.BindLambda([this](UAnimMontage* EndedMontage, bool bInterrupted)
		{
			EndSkill();
		});

	AnimInstancePtr->Montage_SetEndDelegate(MontageEnded, GroundSMashMontage);

	// 다시 Target과 스킬 시전자와의 충돌을 켜준다
	IgnoreCollisionToTarget(Target, false);
}

void AGroundSmash::IgnoreCollisionToTarget(ACharacter* _Target, bool _Enable)
{
	if (nullptr == _Target)
		return;

	UCapsuleComponent* OwnerCapsule = mSkillCaster->GetCapsuleComponent();
	if (nullptr == OwnerCapsule)
		return;
	
	UCapsuleComponent* TargetCapsule = _Target->GetCapsuleComponent();
	if (nullptr == TargetCapsule)
		return;

	OwnerCapsule->IgnoreActorWhenMoving(_Target, _Enable);
	TargetCapsule->IgnoreActorWhenMoving(mSkillCaster, _Enable);
}
