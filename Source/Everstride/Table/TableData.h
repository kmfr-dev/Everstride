// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Common/GameDefines.h"
#include "../Actors/Monster/MonsterBase.h"
#include "../Attack/AttackBase.h"
#include "BehaviorTree/BehaviorTree.h"
#include "TableData.generated.h"


USTRUCT(BlueprintType)
struct FTableLoadRow : public FTableRowBase
{
	GENERATED_BODY()

	FTableLoadRow() {}
	~FTableLoadRow() {}

	// 테이블 경로
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString TablePath = "";

	// 이 테이블을 로드할건지에 대한 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsLoad = false;
};

// 애니메이션 테이블
USTRUCT(BlueprintType)
struct FAnimationTableRow : public FTableRowBase
{
	GENERATED_BODY()

	FAnimationTableRow() {}
	~FAnimationTableRow() {}

	//사용할 애님 인스턴스 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> Animation = nullptr;

	//몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FGameplayTag, TObjectPtr<UAnimMontage>> MontageMap;
};

// 메인 위젯 테이블
USTRUCT(BlueprintType)
struct FMainWidgetTableRow : public FTableRowBase
{
	GENERATED_BODY()

	FMainWidgetTableRow() {}
	~FMainWidgetTableRow() {}

	// 메인 위젯 블루프린트 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> MainWidgetClass = nullptr;
};

// Define 테이블
USTRUCT(BlueprintType)
struct FDefineTableRow : public FTableRowBase
{
	GENERATED_BODY()

	FDefineTableRow() {}
	~FDefineTableRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 IntVal = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FloatVal = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString StrVal = "";

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector VecVal = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator RotVal = FRotator::ZeroRotator;
};

// 스켈레탈 메쉬 테이블
USTRUCT(BlueprintType)
struct FSkeletalMeshTableRow : public FTableRowBase
{
	GENERATED_BODY()

	FSkeletalMeshTableRow() {}
	~FSkeletalMeshTableRow() {}

	// 사용 할 스켈레탈 메쉬
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* SkeletalMesh = nullptr;

	// 메쉬 위치값
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector MeshLocation = FVector::ZeroVector;

	// 메쉬 회전값
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator MeshRotation = FRotator::ZeroRotator;
};

USTRUCT(BlueprintType)
struct FPlayerTableRow : public FTableRowBase
{
	GENERATED_BODY()

	FPlayerTableRow() {}
	~FPlayerTableRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAttackBase> NormalAttackClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName StatTID;

	// 플레이어가 사용 할 충돌TID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CollisionTID;

	//플레이어가 사용 할 스킬TID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> SkillTIDs;

	//플레이어가 사용 할 스켈레탈 메쉬
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkeletalMeshTID;

	//플레이어가 기본으로 사용할 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AnimationTID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TagTID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TeamID;
};

USTRUCT(BlueprintType)
struct FStatTableRow : public FTableRowBase
{
	GENERATED_BODY()

	// 기본으로 사용할 능력치
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDefaultStatData> Status;
};

USTRUCT(BlueprintType)
struct FMapTableRow : public FTableRowBase
{
	GENERATED_BODY()

	FMapTableRow() {}
	~FMapTableRow() {}

	// 맵에 스폰시킬 TID, 몬스터 여러개가 될 수도있으니 배열로 관리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> SpawnTIDs;
};

USTRUCT(BlueprintType)
struct FSpawnTableRow : public FTableRowBase
{
	GENERATED_BODY()

	FSpawnTableRow() {}
	~FSpawnTableRow() {}

	// 스폰할 오브젝트의 테이블 ID, 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SpawnObjectTID;

	// 스폰 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SpawnPosition;
	
	// 스폰 회전
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator SpawnRotation;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnInterval;
};

USTRUCT(BlueprintType)
struct FMonsterTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
	FMonsterTableRow() {}
	~FMonsterTableRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MonsterName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAttackBase> NormalAttackClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName StatTID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AMonsterBase> MonsterClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkeletalMeshTID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AnimationTID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AITID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CollisionTID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> SkilITIDs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TagTID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TeamID = -1;
};


USTRUCT(BlueprintType)
struct FSkillTableRow : public FTableRowBase
{
	GENERATED_BODY()

	FSkillTableRow() {}
	~FSkillTableRow() {}

	// 스킬이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkillName = NAME_None; 

	// 스킬 즉발인지, 특정 타이밍에 사용할건지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillActionType SkillActionType = ESkillActionType::SKILLACTION_TYPE_NONE;

	// 특정 타이밍에 스킬 종료를 할 것인지 로직으로 제어할 것 인지.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool SkillEndByLogic = false;

	// 스킬 풀링할건지 풀링 안할건지
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESkillPoolingType SkillPoolType = ESkillPoolingType::SKILLPOLLING_TYPE_NONE;

	// 스킬 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class ASkillBase> SkillClass;
	
	// 스킬 태그
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag SkillTag = FGameplayTag::EmptyTag;

	// 스킬 이미지 텍스처 테이블 ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TextureTID = NAME_None;

	// 스킬 Value (ex: 데미지, 버프면 증가량 등)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value = 0.f;

	// 스킬 최소거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinRange = 0.f;

	// 스킬 최대거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxRange = 0.f;

	// 쿨타임
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CoolDown = 0.f;

	// 스킬 지속시간, 시전시간
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CastTime = 0.f;

	// 스킬의 자원 소모량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillCost = 0.f;
};

// 충돌 테이블
USTRUCT(BlueprintType)
struct FCollisionTableRow : public FTableRowBase
{
	GENERATED_BODY()

	FCollisionTableRow() {}
	~FCollisionTableRow() {}

	//충돌 프로파일이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Profile;

	//충돌 채널이름
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Channel = -1;
};

USTRUCT(BlueprintType)
struct FCameraArmTableRow : public FTableRowBase
{
	GENERATED_BODY()

	FCameraArmTableRow() {}
	~FCameraArmTableRow() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxLength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinLength = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInheritPitch = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInheritYaw = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInheritRoll = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool CameraLagEnable = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraLagSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CameraRotationLagSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ProveSize = 0.f;
};

USTRUCT(BlueprintType)
struct FAITableRow : public FTableRowBase
{
	GENERATED_BODY()

	// 비헤이비어 트리 에셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBehaviorTree* Tree = nullptr;

	// 블랙보드 에셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBlackboardData* Board = nullptr;

	// AI Perception
	// 몬스터가 시야를 통해 적을 감지할 수 있는 최대거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SightRadius = 0.f;

	// 몬스터가 시야에서 대상을 놓치게 되는 거리
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LoseSightRadius = 0.f;

	// 몬스터의 시야각도 (원뿔형태)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PeripheralVisionAngleDegrees = 0.f;

	// 몬스터가 적을 감지할지에 대한 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDetectEnemies = false;

	// 몬스터가 아군을 감지할지에대한 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDetectFriendlies = false;

	// 적,아군 제외한 대상도 감지할지에대한 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDetectNeutrals = false;
};

// 태그 관련 테이블
USTRUCT(BlueprintType)
struct FTagTableRow : public FTableRowBase
{
	GENERATED_BODY()

	// 태그가 여러개일 경우를 대비해 배열로 선언
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Tags;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCharacterActionTag> ActionTagData;
};

USTRUCT(BlueprintType)
struct FCameraShakeRow : public FTableRowBase
{
	GENERATED_BODY()

	// 실제 카메라 쉐이크 에셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UCameraShakeBase> ShakeClass;

	// 기본 강도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Scale = 1.0f;

	// 연속 재생 방지용
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Cooldown = 0.1f;
};

USTRUCT(BlueprintType)
struct FTextureTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D* Texture = nullptr;
};

UCLASS()
class EVERSTRIDE_API UTableData : public UObject
{
	GENERATED_BODY()

public:
	class TableName
	{
	public:
		static const FName MAP;
		static const FName MAINWIDGET;
		static const FName PLAYER;
		static const FName MONSTER;
		static const FName SKELETALMESH;
		static const FName SPAWN;
		static const FName STAT;
		static const FName SKILL;
		static const FName ANIMATION;
		static const FName AI;
		static const FName CAMERA_ARM;
		static const FName CAMERA_SHAKE;
		static const FName COLLISION;
		static const FName TAG;
		static const FName TEXTURE;
		static const FName DEFINE;
	};

	class DefineName
	{
	public:
		static const FName DEFAULT_PLAYER;
		static const FName CAMERA_ARM;
		static const FName SPRINT_ARMLENGTH;
		static const FName JUMPTHRESHOLD;
		static const FName RUNTHRESHOLD;
	};

	class MainWidgetName
	{
	public:
		static const FName TITLE;
		static const FName INGAME;
	};
};
