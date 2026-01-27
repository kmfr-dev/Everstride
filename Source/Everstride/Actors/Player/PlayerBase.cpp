// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

#include "InputAction.h"
#include "InGamePlayerState.h"
#include "Animation/AnimMontage.h"
#include "../../DataAsset/CharacterActionTag.h"
#include "../../Component/StatComponent.h"
#include "../../Component/PlayerCombatComponent.h"
#include "../../Component/SkillComponent.h"
#include "../../Animation/AnimInstanceBase.h"
#include "../../Input/PlayerInputSystem.h"
#include "../../Subsystem/TableSubsystem.h"
#include "../../Input/InGamePlayerController.h"
#include "NiagaraComponent.h"

APlayerBase::APlayerBase()
{
	mArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));


	// 카메라가 컨트롤러의 회전을 따라가게 설정한다.
	mArm->bUsePawnControlRotation = true;
	mArm->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	// 스프링 암 끝 지점에 카메라 부착
	mCamera->SetupAttachment(mArm, USpringArmComponent::SocketName);

	// 캐릭터가 컨트롤러의 회전을 따라가지 않음
	// 캐릭터 Root(캡슐) 및 그 자식 컴포넌트들(메쉬, 콜리전 등)은
	// 컨트롤러 회전을 무시하고 자체 회전 로직(예: 이동 방향 기반 회전)을 사용
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 캐릭터가 이동하는 방향을 바라보도록 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	// 무기메쉬 설정
	mWeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	mWeaponMesh->SetCollisionProfileName(TEXT("NoCollision"));
	mWeaponMesh->SetupAttachment(GetMesh(), TEXT("Weapon_r"));

	static ConstructorHelpers::FObjectFinder<UStaticMesh>
		PlayerWeaponRef(TEXT("/Script/Engine.StaticMesh'/Game/Asset/Character/Dark_Knight/Meshes/SM_DKM_Sword.SM_DKM_Sword'"));
	if (PlayerWeaponRef.Succeeded())
		mWeaponMesh->SetStaticMesh(PlayerWeaponRef.Object);

	mCombatComp = CreateDefaultSubobject<UPlayerCombatComponent>("Combat");
}

void APlayerBase::BeginPlay()
{
	Super::BeginPlay();

	
}

void APlayerBase::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	mWeaponMesh->SetVisibility(true);
}

void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (nullptr == mInput)
		mInput = NewObject<UPlayerInputSystem>(this);

	// 기존 InputComponent를 EnhancedInputComponent로 전환
	UEnhancedInputComponent* InputCompPtr = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (false == IsValid(InputCompPtr))
		return;

	APlayerController* PlayerControllerPtr = GetController<APlayerController>();
	if (nullptr == PlayerControllerPtr)
		return;

	// PlayerController에 값을 지정
	UEnhancedInputLocalPlayerSubsystem* InputSub =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerControllerPtr->GetLocalPlayer());

	InputSub->AddMappingContext(mInput->GetContext(), 0);
	// 캐릭터 이동 함수 바인딩
	InputCompPtr->BindAction(mInput->GetMoveAction(), ETriggerEvent::Triggered, this, &APlayerBase::MoveAction);
	// 카메라 회전 함수 바인딩
	InputCompPtr->BindAction(mInput->GetCameraRotAction(), ETriggerEvent::Triggered, this, &APlayerBase::CameraRotationAction);
	// 달리기 시작, 종료 함수 바인딩
	InputCompPtr->BindAction(mInput->GetSprintAction(), ETriggerEvent::Started, this, &APlayerBase::SprintAction);
	InputCompPtr->BindAction(mInput->GetSprintAction(), ETriggerEvent::Completed, this, &APlayerBase::StopSprintAction);
	// 공격 함수 바인딩
	InputCompPtr->BindAction(mInput->GetAttackAction(), ETriggerEvent::Started, this, &APlayerBase::AttackAction);
	// 점프 함수 바인딩
	InputCompPtr->BindAction(mInput->GetJumpAction(), ETriggerEvent::Started, this, &APlayerBase::JumpAction);
	// 검기 휘두르는 스킬 함수 바인딩
	InputCompPtr->BindAction(mInput->GetSlashSkillAction(), ETriggerEvent::Started, this, &APlayerBase::SlashSkillAction);
	// 닷지 스킬 함수 바인딩
	InputCompPtr->BindAction(mInput->GetDodgeSkillAction(), ETriggerEvent::Started, this, &APlayerBase::DodgeSkillAction);
}

void APlayerBase::Dead()
{
	Super::Dead();

	mWeaponMesh->SetVisibility(false);

}

void APlayerBase::DefaultSetting()
{
	UTableSubsystem* TableSub = UTableSubsystem::Get(this);
	if (nullptr == TableSub)
		return;

	AInGamePlayerState* PlayerStatePtr = GetPlayerState<AInGamePlayerState>();
	if (nullptr == PlayerStatePtr)
		return;

	const FName& PlayerTID = PlayerStatePtr->GetPlayerTID();
	
	FPlayerTableRow* PlayerRow = TableSub->FindTableRow<FPlayerTableRow>(UTableData::TableName::PLAYER, PlayerTID);
	if (nullptr == PlayerRow)
		return;

	AInGamePlayerController* PlayerControllerPtr = GetController<AInGamePlayerController>();
	if (nullptr == PlayerControllerPtr)
		return;

	PlayerControllerPtr->SetGenericTeamId(PlayerRow->TeamID);

	InitAttack(PlayerRow->NormalAttackClass);

	InitStatus(PlayerRow->StatTID);

	InitMesh(TableSub, PlayerRow->SkeletalMeshTID);

	InitAnimInstance(TableSub , PlayerRow->AnimationTID);

	InitTag(TableSub, PlayerRow->TagTID);

	InitCollision(TableSub, PlayerRow->CollisionTID);

	// Define테이블에 정의된 카메라 암 관련 행을 얻어온다.
	FDefineTableRow* DefineRow = TableSub->FindTableRow<FDefineTableRow>(UTableData::TableName::DEFINE, UTableData::DefineName::CAMERA_ARM);
	if (nullptr == DefineRow)
		return;

	InitCamera(TableSub, FName(DefineRow->StrVal));
	
	InitMovement();

	for (const FName& SkillID : PlayerRow->SkillTIDs)
		InitSkill(TableSub, SkillID);

	FOnSkillFinished OnSkillFinished;
	OnSkillFinished.BindUObject(this, &APlayerBase::SkillActionEnd);
	mSkillComp->SetSkillFinished(OnSkillFinished);
}

void APlayerBase::InitUI()
{
	AInGamePlayerState* PlayerStatePtr = GetPlayerState<AInGamePlayerState>();
	if (nullptr == PlayerStatePtr)
		return;

	const FName& PlayerTID = PlayerStatePtr->GetPlayerTID();
	OnInitUI.Broadcast(PlayerTID);

	float MaxHP = mStatComponent->GetTotalStatValue(EStatusType::STATTYPE_MAXHP);
	float CurrentHP = mStatComponent->GetTotalStatValue(EStatusType::STATTYPE_HP);

	UpdateHPBar(CurrentHP, MaxHP);
	//OnStatUpdate.Execute(CurrentHP, MaxHp);
	//OnStatUpdate.
}

void APlayerBase::UpdateHPBar(float _CurrentHP, float _MaxHP)
{
	OnStatUpdate.Execute(_CurrentHP, _MaxHP);
}

void APlayerBase::InitCamera(UTableSubsystem* _TableSub, const FName& _TID)
{
	if (nullptr == _TableSub)
		return;

	// 카메라암 테이블 조회
	FCameraArmTableRow* ArmRow = _TableSub->FindTableRow<FCameraArmTableRow>(UTableData::TableName::CAMERA_ARM, _TID);
	if (nullptr == ArmRow)
		return;

	// ===== 카메라암 테이블 행에 정의된 값으로 카메라암 세팅 =====
	mArm->TargetArmLength = (ArmRow->MaxLength + ArmRow->MinLength) / 2;
	mArm->SetRelativeLocation(ArmRow->Location);
	mArm->SetRelativeRotation(ArmRow->Rotation);

	mArm->bInheritPitch = ArmRow->bInheritPitch;
	mArm->bInheritYaw = ArmRow->bInheritYaw;
	mArm->bInheritRoll = ArmRow->bInheritRoll;

	mArm->bDoCollisionTest = true;

	mArm->ProbeSize = ArmRow->ProveSize;

	mArm->bEnableCameraLag = ArmRow->CameraLagEnable;
	mArm->CameraLagSpeed = ArmRow->CameraLagSpeed;
	mArm->CameraRotationLagSpeed = ArmRow->CameraRotationLagSpeed;
	// ============================================================
}

void APlayerBase::MoveAction(const FInputActionValue& _InputActionValue)
{
	if (mAnim->IsAnyMontagePlaying())
		return;

	FVector Axis;

	// 떨어지는 경우 입력으로 가해지는 속도를 0.1배
	if (GetCharacterMovement()->IsFalling())
		Axis = _InputActionValue.Get<FVector>() * 0.1f;
	else
		Axis = _InputActionValue.Get<FVector>();

	const FRotator Rot = Controller->GetControlRotation();
	const FRotator YawRot = FRotator(0.f, Rot.Yaw, 0.f);

	FVector ForwardVec = FRotationMatrix(YawRot).GetUnitAxis(EAxis::X);
	FVector RightVec = FRotationMatrix(YawRot).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardVec, Axis.X);
	AddMovementInput(RightVec, Axis.Y);
}

void APlayerBase::CameraRotationAction(const FInputActionValue& _InputActionValue)
{
	// 컨트롤러가 없을 경우 리턴
	FVector2D AxisVector2D = _InputActionValue.Get<FVector2D>();

	FRotator CurrentControlRot = Controller->GetControlRotation();
	CurrentControlRot.Normalize();

	// Yaw는 그대로 더함
	CurrentControlRot.Yaw += AxisVector2D.X;

	// Pitch는 직접 clamp해서 더함
	float NewPitch = CurrentControlRot.Pitch + AxisVector2D.Y;
	CurrentControlRot.Pitch = FMath::Clamp(NewPitch, -80.f, 30.f);

	Controller->SetControlRotation(CurrentControlRot);
}

void APlayerBase::SprintAction(const FInputActionValue& _InputActionValue)
{
	if (false == IsValid(mStatComponent))
		return;

	// 현재 이동속도를 얻어옴
	float CurrentSpeed = mStatComponent->GetStatValue(EStatusType::STATTYPE_SPEED, EStatusCategory::STATCATEGORY_TOTAL);

	// 스탯 컴포넌트에서 속도 비율을 얻어온다.
	float RunRatio = mStatComponent->GetStatValue(EStatusType::STATTYPE_RUNRATIO, EStatusCategory::STATCATEGORY_BASE);

	// 현재 이동속도에 달리기 비율을 곱해 현재속도를 계산
	CurrentSpeed = CurrentSpeed * RunRatio;

	// 스탯 컴포넌트에 토탈값을 설정
	mStatComponent->SetStatValue(EStatusType::STATTYPE_SPEED, EStatusCategory::STATCATEGORY_TOTAL, CurrentSpeed);

	// 이동속도 업데이트
	UpdateMovementSpeed();
}

void APlayerBase::StopSprintAction(const FInputActionValue& _InputActionValue)
{
	if (false == IsValid(mStatComponent))
		return;

	UpdateBaseSpeed();
}

void APlayerBase::AttackAction(const FInputActionValue& _InputActionValue)
{
	if (IsValid(mCombatComp))
	{
		mCombatComp->AttackBegin();
	}
}

void APlayerBase::JumpAction(const FInputActionValue& _InputActionValue)
{
	if (false == IsValid(mCombatComp))
		return;

	if (CanJump() && false == mCombatComp->IsCombat())
	{
		Jump();
		mWeaponMesh->SetVisibility(false);
	}
}

void APlayerBase::SlashSkillAction(const FInputActionValue& _InputActionValue)
{
	FSkillMappingData* MappingData = GetSkillMappingData(EverstrideGamePlayTags::TAG_Skill_Player_Slash);
	if (nullptr == MappingData)
		return;

	SkillAction(MappingData->SkillID);
}

void APlayerBase::DodgeSkillAction(const FInputActionValue& _InputActionValue)
{
	FSkillMappingData* MappingData = GetSkillMappingData(EverstrideGamePlayTags::TAG_Skill_Player_Dodge);
	if (nullptr == MappingData)
		return;

	SkillAction(MappingData->SkillID);
}

void APlayerBase::UpdateBaseSpeed()
{
	// 기본 이동속도를 얻어옴
	float BaseSpeed = mStatComponent->GetStatValue(EStatusType::STATTYPE_SPEED, EStatusCategory::STATCATEGORY_BASE);

	// 토탈 이동속도를 기본이동 속도로 값으로 업데이트
	mStatComponent->SetStatValue(EStatusType::STATTYPE_SPEED, EStatusCategory::STATCATEGORY_TOTAL, BaseSpeed);

	// 이동속도 업데이트
	UpdateMovementSpeed();
}

void APlayerBase::UpdateMovementSpeed()
{
	// 무브먼트 컴포넌트를 얻어온다.
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (false == IsValid(MoveComp))
		return;

	// 스탯 컴포넌트에서 이동속도 Total값을 가져온다.
	float MoveSpeed = mStatComponent->GetStatValue(EStatusType::STATTYPE_SPEED, EStatusCategory::STATCATEGORY_TOTAL);

	// 이동속도 업데이트
	MoveComp->MaxWalkSpeed = MoveSpeed;
}

void APlayerBase::SkillAction(const FName& _ID)
{
	if (false == IsValid(mSkillComp))
		return;

	// 스킬 선택에 성공하면 바로 스킬 액션 진행
	bool SuccsessChoose = mSkillComp->ChooseSkill(_ID);
	if (false == SuccsessChoose)
		return;

	mSkillComp->SkillAction(_ID);
}

void APlayerBase::SkillActionEnd()
{
	if (false == IsValid(mSkillComp))
		return;

	const FName& SkillID = mSkillComp->GetCastingSkill();
	OnUsedSkill.Execute(SkillID);
}

UCombatComponent* APlayerBase::GetCombatComponent()
{
	if (IsValid(mCombatComp))
		return mCombatComp;

	return nullptr;
}