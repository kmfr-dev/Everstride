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
	FString TablePath;

	// 이 테이블을 로드할건지에 대한 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsLoad = false;
};

USTRUCT(BlueprintType)
struct FAnimationTableRow : public FTableRowBase
{
	GENERATED_BODY()

	FAnimationTableRow() {}
	~FAnimationTableRow() {}

	//사용할 애니메이션인스턴스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> Animation = nullptr;

	//시퀀스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EMotionType, TObjectPtr<UAnimSequence>> AnimMap;

	//몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<EMontageType, TObjectPtr<UAnimMontage>> MontageMap;
};

USTRUCT(BlueprintType)
struct FMainWidgetTableRow : public FTableRowBase
{
	GENERATED_BODY()

	FMainWidgetTableRow() {}
	~FMainWidgetTableRow() {}

	// 메인 위젯 블루프린트 클래스
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> MainWidgetClass;
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
		static const FName ITEM;
		static const FName SKELETALMESH;
		static const FName ANIMATION;
		static const FName CAMERA_ARM;
		static const FName COLLISION;
		static const FName DEFINE;
	};

	class MainWidgetName
	{
	public:
		static const FName TITLE;
		static const FName INGAME;
	};
};
