// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Common/GameDefines.h"
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

	//시퀀스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EMotionType, TObjectPtr<UAnimSequence>> AnimMap;

	//몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EMontageType, TObjectPtr<UAnimMontage>> MontageMap;
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

	// 플레이어가 기본으로 사용할 능력치
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDefaultStatData> Status;

	// 플레이어가 사용 할 충돌TID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName CollisionTID;

	//플레이어가 사용 할 스킬TID
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkillTID;

	//플레이어가 사용 할 스켈레탈 메쉬
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkeletalMeshTID;

	//플레이어가 기본으로 사용할 애니메이션
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName AnimationTID;
};


//USTRUCT(BlueprintType)
//struct FSkillDataTableRow : public FTableRowBase
//{
//	GENERATED_BODY()
//
//	FSkillDataTableRow() {}
//	~FSkillDataTableRow() {}
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	FName SkillName;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	ESkillType SkillType;
//
//	// 패시브, 버프 스킬로 인한 스탯 증가 시 사용
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	TArray<FStatusData> SkillStatBoost;
//
//	//// 패시브 스킬의 경우 소지할 아이템의 TID 배열
//	//TArray<FName> GetItemTIDs;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	float SkillCoolTime;
//
//	// 스킬 지속시간 or 증가량 
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	float SkillDuration;
//
//	// 스킬 UI TID
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	FName SkillViewTID;
//
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	TSubclassOf<class ASKillBase> SkillClass;
//
//	// 나이아가라 이펙트 에셋
//	UPROPERTY(EditAnywhere, BlueprintReadWrite)
//	TObjectPtr<class UNiagaraSystem> SkillEffect;
//};

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
	int32 Channel;
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

UCLASS()
class EVERSTRIDE_API UTableData : public UObject
{
	GENERATED_BODY()

public:
	class TableName
	{
	public:
		static const FName MAINWIDGET;
		static const FName PLAYER;
		static const FName SKELETALMESH;
		static const FName ANIMATION;
		static const FName CAMERA_ARM;
		static const FName COLLISION;
		static const FName DEFINE;
	};

	class DefineName
	{
	public:
		static const FName DEFAULT_PLAYER;
		static const FName CAMERA_ARM;
	};

	class MainWidgetName
	{
	public:
		static const FName TITLE;
		static const FName INGAME;
	};
};
