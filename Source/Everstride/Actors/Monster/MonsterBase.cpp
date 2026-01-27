// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterBase.h"
#include "../../Subsystem/TableSubsystem.h"
#include "../../Animation/AnimInstanceBase.h"
#include "Controller/AIControllerMonster.h"
#include "../../Component/StatComponent.h"
#include "../../Component/MonsterCombatComponent.h"
#include "../../DataAsset/CharacterActionTag.h"
#include "Components/WidgetComponent.h"
#include "../../UI/InGame/StatBarWidget.h"

AMonsterBase::AMonsterBase()
{
	AIControllerClass = AAIControllerMonster::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	mCombatComp = CreateDefaultSubobject<UMonsterCombatComponent>("Combat");
	
	mHPBar = CreateDefaultSubobject<UWidgetComponent>("HPBar");
	mHPBar->SetupAttachment(GetMesh(), TEXT("HealthBar"));
	mHPBar->SetWidgetSpace(EWidgetSpace::Screen);
	mHPBar->SetDrawSize(FVector2D(150.f, 30.f));
	mHPBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	static ConstructorHelpers::FClassFinder<UUserWidget>
		WidgetClassRef(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/02_InGame/WBP_StatBarWidget.WBP_StatBarWidget_C'"));
	if (WidgetClassRef.Succeeded())
		mHPBar->SetWidgetClass(WidgetClassRef.Class);
}

void AMonsterBase::BeginPlay()
{
	Super::BeginPlay();
	
	AAIControllerMonster* ControllerPtr = GetController<AAIControllerMonster>();
	if (nullptr == ControllerPtr)
		return;

	ControllerPtr->SetBlackBoardData();
}

void AMonsterBase::DefaultSetting() 
{
	UTableSubsystem* TableSub = UTableSubsystem::Get(this);
	if (nullptr == TableSub)
		return;

	FMonsterTableRow* MonsterRow = TableSub->FindTableRow<FMonsterTableRow>(UTableData::TableName::MONSTER, mTID);
	if (nullptr == MonsterRow)
		return;

	mMonsterName = MonsterRow->MonsterName;

	InitAttack(MonsterRow->NormalAttackClass);

	InitStatus(MonsterRow->StatTID);

	InitMesh(TableSub, MonsterRow->SkeletalMeshTID);

	InitAnimInstance(TableSub, MonsterRow->AnimationTID);

	InitTag(TableSub, MonsterRow->TagTID);

	InitCollision(TableSub, MonsterRow->CollisionTID);

	for (const FName& SkillID : MonsterRow->SkilITIDs)
		InitSkill(TableSub, SkillID);
}

void AMonsterBase::UpdateHPBar(float _CurrentHP, float _MaxHP)
{
	if (nullptr == mHPBar)
		return;

	UStatBarWidget* WidgetPtr = Cast<UStatBarWidget>(mHPBar->GetUserWidgetObject());
	if (nullptr == WidgetPtr)
		return;

	WidgetPtr->SetCurrentStatText(_CurrentHP);
	WidgetPtr->SetMaxStatText(_MaxHP);
	
	const float NewPercent = _CurrentHP / _MaxHP;
	WidgetPtr->SetStatPercent(NewPercent);
}

void AMonsterBase::AttackEnd(UAnimMontage* _TargetMontage, bool _IsProperyEnded)
{
	UAnimInstanceBase* AnimInst = Cast<UAnimInstanceBase>(GetMesh()->GetAnimInstance());
	if (nullptr == AnimInst)
		return;

	UAnimMontage* MontagePtr = AnimInst->GetAnimMontage(mTagData->AttackTag);
	if (nullptr == MontagePtr)
		return;

	if (MontagePtr != _TargetMontage)
		return;

	OnAttackFinished.Execute();
}

void AMonsterBase::Dead()
{
	Super::Dead();

	EnableHPBar(false);

	AAIControllerBase* AIControllerPtr = GetController<AAIControllerBase>();
	if (nullptr == AIControllerPtr)
		return;

	AIControllerPtr->StopBehaviorTree();

	FTimerHandle DeathTimer;
	GetWorld()->GetTimerManager().SetTimer(DeathTimer, 
		FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), 5.f, false);
}

void AMonsterBase::Hit(AActor* _HitActor)
{
	Super::Hit(_HitActor);

	if (nullptr == _HitActor)
		return;

	if (nullptr == mHPBar || nullptr == mHPBar->GetWidgetClass())
		return;

	AAIControllerMonster* AIControllerPtr = GetController<AAIControllerMonster>();
	if (nullptr == AIControllerPtr)
		return;

	AActor* Target = AIControllerPtr->GetTarget();
	if (nullptr == Target)
	{
		FVector HitActorDir = _HitActor->GetActorLocation() - GetActorLocation();
		HitActorDir.Z = 0.f;
		HitActorDir.Normalize();

		const FRotator& LookRot = HitActorDir.Rotation();
		SetActorRotation(LookRot);
 	}
}

void AMonsterBase::InitUI()
{
	if (nullptr == mHPBar || false == mHPBar->IsVisible())
		return;

	UStatBarWidget* WidgetPtr = Cast<UStatBarWidget>(mHPBar->GetUserWidgetObject());
	if (nullptr == WidgetPtr)
		return;

	WidgetPtr->SetStatBarColor(EStatBarType::STATBAR_TYPE_HP);

	UStatComponent* StatComp = GetStatComponent();
	if (nullptr == StatComp)
		return;

	const float MaxHP = StatComp->GetTotalStatValue(EStatusType::STATTYPE_MAXHP);
	const float CurHP = StatComp->GetTotalStatValue(EStatusType::STATTYPE_HP);

	UpdateHPBar(CurHP, MaxHP);

	mHPBar->SetVisibility(false);
}

void AMonsterBase::SetAIAttackDelegate(const FAIAttackFinished& _InAttackFinished)
{
	
	OnAttackFinished = _InAttackFinished;
} 

void AMonsterBase::AttackByAI()
{
	UAnimInstanceBase* AnimInstancePtr = Cast<UAnimInstanceBase>(GetMesh()->GetAnimInstance());
	if (nullptr == AnimInstancePtr)
		return;

	UAnimMontage* Montage = AnimInstancePtr->GetAnimMontage(EverstrideGamePlayTags::TAG_AnimMontage_Monster_Attack);
	if (nullptr == Montage)
		return;

	bool IsPlayingMontage = AnimInstancePtr->Montage_IsPlaying(Montage);
	if (IsPlayingMontage)
		return;

	AnimInstancePtr->PlayAnimMontage(mTagData->AttackTag);

	FOnMontageEnded OnMontageEnded;
	OnMontageEnded.BindUObject(this, &AMonsterBase::AttackEnd);
	AnimInstancePtr->Montage_SetEndDelegate(OnMontageEnded);
}

float AMonsterBase::GetAttackRange()
{
	if (false == IsValid(mStatComponent))
		return 0;

	float AttackRange = mStatComponent->GetTotalStatValue(EStatusType::STATTYPE_ATTACKRANGE);
	
	float AttackRadius = mStatComponent->GetTotalStatValue(EStatusType::STATTYPE_ATTACKRADIUS);
	AttackRadius *= 2;

	return AttackRange + AttackRadius;
}

void AMonsterBase::EnableHPBar(bool _Enable)
{
	if (nullptr == mHPBar || nullptr == mHPBar->GetWidgetClass())
		return;

	mHPBar->SetVisibility(_Enable);
}
