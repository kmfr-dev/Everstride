#include "CharacterBase.h"
#include "InputAction.h"
#include "../DataAsset/CharacterActionTag.h"
#include "../Component/StatComponent.h"
#include "../Animation/AnimInstanceBase.h"
#include "../Attack/AttackBase.h"
#include "../Subsystem/TableSubsystem.h"
#include "../Component/CombatComponent.h"
#include "../Component/SkillComponent.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	mStatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("Status"));
	mSkillComp = CreateDefaultSubobject<USkillComponent>("Skill");
	mSkeletalMesh = GetMesh();

	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();

	DefaultSetting();

	InitUI();

	InitMovement();
}

float ACharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// 방어력을 얻어온다.
	const float Defence = mStatComponent->GetTotalStatValue(EStatusType::STATTYPE_DEFENCE);
	
	// 최종 데미지 계산
	const float DamageToApply = FMath::Max(0.0f, DamageAmount - Defence);

	// 현재 체력을 얻어온다.
	float CurrentHP = mStatComponent->GetTotalStatValue(EStatusType::STATTYPE_HP);

	// 현재 체력 차감
	CurrentHP = FMath::Max(0.0f, CurrentHP - DamageToApply);
	mStatComponent->SetStatValue(EStatusType::STATTYPE_HP, EStatusCategory::STATCATEGORY_TOTAL, CurrentHP);

	const float MaxHP = mStatComponent->GetTotalStatValue(EStatusType::STATTYPE_MAXHP);

	UpdateHPBar(CurrentHP, MaxHP);

	// 만약 0보다 작을경우 죽은걸로 판정
	if (CurrentHP <= 0.0f)
	{
		Dead();
	}

	// 아닐경우
	else
	{
		Hit(DamageCauser);
	}

	return DamageToApply;
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacterBase::DefaultSetting()
{
	// Nothing
}

void ACharacterBase::InitUI()
{
}

void ACharacterBase::InitMesh(UTableSubsystem* _TableSub, const FName& _TID)
{
	if (nullptr == _TableSub)
		return;

	// SkeletalTID로 스켈레탈 테이블 조회 
	FSkeletalMeshTableRow* SkeletalRow = _TableSub->FindTableRow<FSkeletalMeshTableRow>(UTableData::TableName::SKELETALMESH, _TID);
	if (nullptr == SkeletalRow)
		return;

	// 스켈레탈 메쉬 지정
	mSkeletalMesh->SetSkeletalMeshAsset(SkeletalRow->SkeletalMesh);
	// 스켈레탈 메쉬 위치및 회전값 지정
	mSkeletalMesh->SetRelativeLocation(SkeletalRow->MeshLocation);
	mSkeletalMesh->SetRelativeRotation(SkeletalRow->MeshRotation);
}

void ACharacterBase::InitAnimInstance(UTableSubsystem* _TableSub, const FName& _TID)
{
	if (nullptr == _TableSub)
		return;

	// AnimTID 로 애님 테이블을 조회한다
	FAnimationTableRow* AnimRow = _TableSub->FindTableRow<FAnimationTableRow>(UTableData::TableName::ANIMATION, _TID);
	if (nullptr == AnimRow)
		return;

	// 애님 클래스를 해당행으로 부터 얻어온다.
	TSubclassOf<UAnimInstance> AnimClassFromTable = AnimRow->Animation;
	if (nullptr == AnimClassFromTable)
		return;

	// 스켈레탈 메시 컴포넌트에 로드한 AnimInstance 클래스를 설정
	mSkeletalMesh->SetAnimInstanceClass(AnimClassFromTable);

	// 메시 컴포넌트에 의해 생성된 AnimInstance를 가져와 참조
	UAnimInstanceBase* AnimInsPtr = Cast<UAnimInstanceBase>(mSkeletalMesh->GetAnimInstance());
	if (nullptr == AnimInsPtr)
		return;

	mAnim = AnimInsPtr;
	mAnim->SetAnimData(*AnimRow);
}

void ACharacterBase::InitAttack(TSubclassOf<UAttackBase> _Class)
{
	if (nullptr == _Class)
		return;

	if (false == IsValid(mCombatComp))
		return;

	mCombatComp->SetAttackData(_Class);
}

void ACharacterBase::InitTag(UTableSubsystem* _TableSub, const FName& _TID)
{
	if (nullptr == _TableSub)
		return;

	FTagTableRow* TagRow = _TableSub->FindTableRow<FTagTableRow>(UTableData::TableName::TAG, _TID);
	if (nullptr == TagRow)
		return;

	for (const FName& Tag : TagRow->Tags)
		Tags.Add(Tag);

	mTagData = TagRow->ActionTagData;
}

void ACharacterBase::InitCollision(UTableSubsystem* _TableSub, const FName& _TID)
{
	if (nullptr == _TableSub)
		return;

	FCollisionTableRow* CollisionRow = _TableSub->FindTableRow<FCollisionTableRow>(UTableData::TableName::COLLISION, _TID);
	if (nullptr == CollisionRow)
		return;

	GetCapsuleComponent()->SetCollisionProfileName(CollisionRow->Profile);
}

void ACharacterBase::InitSkill(UTableSubsystem* _TableSub, const FName& _TID)
{
	mSkillComp->SetSkillData(_TID);

	FSkillTableRow* SkillRow = _TableSub->FindTableRow<FSkillTableRow>(UTableData::TableName::SKILL, _TID);
	if (nullptr == SkillRow)
		return;

	mSkillMappingData.Emplace(FSkillMappingData(SkillRow->SkillTag, _TID));
}

void ACharacterBase::InitStatus (const FName& _TID)
{
	if (IsValid(mStatComponent))
		mStatComponent->InitStatus(_TID);
}

void ACharacterBase::InitMovement()
{
	UCharacterMovementComponent* MovementComp = GetCharacterMovement();
	if (nullptr == MovementComp || false == IsValid(mStatComponent))
		return;

	// 능력치 컴포넌트에서 이동속도 값을 얻어온다.
	float BaseMoveSpeed = mStatComponent->GetStatValue
	(EStatusType::STATTYPE_SPEED, EStatusCategory::STATCATEGORY_BASE);

	// 기본 이동속도값 설정
	MovementComp->MaxWalkSpeed = BaseMoveSpeed;
}

void ACharacterBase::AttackHitCheck()
{
	if (IsValid(mCombatComp))
		mCombatComp->AttackHitCheck();
}

void ACharacterBase::Dead()
{
	if (IsValid(mAnim))
	{
		mDead = true;	
		
		// 이동을 막는다.
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
		// 루트 콜리전을 꺼준다.
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		// 모든 애님 몽타주를 스탑
		mAnim->StopAllMontages(0.0f);
		// 죽는 애님 몽타주 재생
		mAnim->PlayAnimMontage(mTagData->DeathTag);

		FOnMontageBlendingOutStarted  MontageEnd;
		MontageEnd.BindLambda(
			[&](UAnimMontage* _Anim, bool _bool)
			{
				if(IsValid(this))
				{
					// 피직스를 켜주고 래그돌 활성화
					mSkeletalMesh->bPauseAnims = true;
					mSkeletalMesh->WakeAllRigidBodies();
					mSkeletalMesh->SetSimulatePhysics(true);
					mSkeletalMesh->SetCollisionProfileName(TEXT("Ragdoll"));
				}
			}
		);
		
		mAnim->Montage_SetBlendingOutDelegate(MontageEnd, mAnim->GetAnimMontage(mTagData->DeathTag));
	}
}

void ACharacterBase::Hit(AActor* _HitActor)
{
	// 맞는 애니메이션 재생
	mAnim->PlayAnimMontage(mTagData->HitTag);
}

void ACharacterBase::UpdateHPBar(float _CurrentHP, float _MaxHP)
{
	// Nothing
}

UStatComponent* ACharacterBase::GetStatComponent()
{
	if (IsValid(mStatComponent))
		return mStatComponent;

	return nullptr;
}

USkillComponent* ACharacterBase::GetSkillComponent()
{
	if (IsValid(mSkillComp))
		return mSkillComp;

	return nullptr;
}

FSkillMappingData* ACharacterBase::GetSkillMappingData(const FGameplayTag& _Tag)
{
	FSkillMappingData* MappingData =
		mSkillMappingData.FindByKey(_Tag);

	if (nullptr == MappingData)
		return nullptr;

	return MappingData;
}