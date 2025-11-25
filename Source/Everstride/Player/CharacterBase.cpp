#include "CharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstanceBase.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InGamePlayerState.h"
#include "../Input/PlayerInputSystem.h"
#include "../Subsystem/TableSubsystem.h"
#include "Component/StatComponent.h"

ACharacterBase::ACharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	mArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	mCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	mStatComponent = CreateDefaultSubobject<UStatComponent>(TEXT("Status"));


	// 카메라가 컨트롤러의 회전을 따라가게 설정한다.
	mArm->bUsePawnControlRotation = true;
	mArm->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform);

	// 스프링 암 끝 지점에 카메라 부착
	mCamera->SetupAttachment(mArm, USpringArmComponent::SocketName);

	// 캐릭터가 컨트롤러의 회전을 따라가지 않음
	// 캐릭터 Root(캡슐) 및 그 자식 컴포넌트들(메쉬, 콜리전 등)은
	// 컨트롤러 회전을 무시하고 자체 회전 로직(예: 이동 방향 기반 회전)을 사용
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// 캐릭터가 이동하는 방향을 바라보도록 설정
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	mSkeletalMesh = GetMesh();
}

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	DefaultSetting();
}

void ACharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    if(nullptr == mInput)
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

    InputCompPtr->BindAction(mInput->GetMoveAction(), ETriggerEvent::Triggered, this, &ACharacterBase::MoveAction);
    InputCompPtr->BindAction(mInput->GetCameraRotAction(), ETriggerEvent::Triggered, this, &ACharacterBase::CameraRotationAction);
}

void ACharacterBase::DefaultSetting()
{
	InitMesh();

	InitCamera();

	InitMovement();
}

void ACharacterBase::InitMesh()
{
	//기본 테이블 서브시스템을 불러옴
	UTableSubsystem* TableSub = UTableSubsystem::Get(this);
	if (nullptr == TableSub)
		return;

	// 플레이어 스테이트를 얻어온다.
	AInGamePlayerState* PlayerStatePtr = GetPlayerState<AInGamePlayerState>();
	if (nullptr == PlayerStatePtr)
		return;

	// 플레이어 TID를 얻어온다.
	FName PlayerTID = PlayerStatePtr->GetPlayerTID();

	// 플레이어 테이블 조회
	FPlayerTableRow* PlayerTable = TableSub->FindTableRow<FPlayerTableRow>(UTableData::TableName::PLAYER, PlayerTID);
	if (nullptr == PlayerTable)
		return;

	// 플레이어 테이블에있는 SkeletalTID로 스켈레탈 테이블 조회 
	FSkeletalMeshTableRow* SkeletalRow = TableSub->FindTableRow<FSkeletalMeshTableRow>(UTableData::TableName::SKELETALMESH, PlayerTable->SkeletalMeshTID);
	if (nullptr == SkeletalRow)
		return;

	// 스켈레탈 메쉬 지정
	mSkeletalMesh->SetSkeletalMeshAsset(SkeletalRow->SkeletalMesh);
	// 스켈레탈 메쉬 위치및 회전값 지정
	mSkeletalMesh->SetRelativeLocation(SkeletalRow->MeshLocation);
	mSkeletalMesh->SetRelativeRotation(SkeletalRow->MeshRotation);
}

void ACharacterBase::InitCamera()
{
	UTableSubsystem* TableSub = UTableSubsystem::Get(this);
	if (nullptr == TableSub)
		return;

	// Define테이블에 정의된 카메라 암 관련 행을 얻어온다.
	FDefineTableRow* DefineRow = TableSub->FindTableRow<FDefineTableRow>(UTableData::TableName::DEFINE, UTableData::DefineName::CAMERA_ARM);
	if (nullptr == DefineRow)
		return;
	
	// 카메라암 테이블 조회
	FCameraArmTableRow* ArmRow = TableSub->FindTableRow<FCameraArmTableRow>(UTableData::TableName::CAMERA_ARM, FName(DefineRow->StrVal));
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

void ACharacterBase::MoveAction(const FInputActionValue& _InputActionValue)
{
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

void ACharacterBase::CameraRotationAction(const FInputActionValue& _InputActionValue)
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

UStatComponent* ACharacterBase::GetStatComponent()
{
	if (IsValid(mStatComponent))
		return mStatComponent;

	return nullptr;
}

