#pragma once

#include "EngineMinimal.h"
#include "Engine.h"
#include "Engine/DamageEvents.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Object.h"
#include "UObject/NoExportTypes.h"
#include "GameDefines.generated.h"

UENUM()
enum class ELevelType : uint8
{
	LEVEL_TYPE_NONE,
	LEVEL_TYPE_TITLE,
	LEVEL_TYPE_INGAME
};

UENUM()
enum class EStatBarType : uint8
{
	STATBAR_TYPE_NONE,
	STATBAR_TYPE_HP,
	STATBAR_TYPE_STAMINA,
};

// 몽타주 구분
UENUM()
enum class EMontageType : uint8
{
	MONTAGE_TYPE_NONE,
};

UENUM()
enum class EMotionType : uint8
{
	MONTION_TYPE_NONE,
};

// 스탯 타입
UENUM(BlueprintType)
enum class EStatusType : uint8
{
	STATUSTYPE_NONE				UMETA(DisplayName = "None"),
	STATTYPE_SPEED				UMETA(DisplayName = "MoveSpeed"),
	STATTYPE_HP					UMETA(DisplayName = "HP"),
	STATTYPE_MAXHP				UMETA(DisplayName = "MaxHP"),
	STATTYPE_STAMINA			UMETA(DisplayName = "Stamina"),
	STATTYPE_MAXSTAMINA				UMETA(DisplayName = "MaxStamina"),
	STATTYPE_RUNRATIO			UMETA(DisplayName = "RunRatio"),
	STATTYPE_ATTACK				UMETA(DisplayName = "Attack"),
};

UENUM(BlueprintType)
enum class EStatusCategory : uint8
{
	STATCATEGORY_NONE,
	STATCATEGORY_BASE,
	STATCATEGORY_DELTA,
	STATCATEGORY_TOTAL
};

USTRUCT(BlueprintType)
struct FDefaultStatData
{
	GENERATED_BODY()

public:
	FDefaultStatData() {}
	FDefaultStatData(EStatusType _Type, float _Value) : StatType(_Type), Value(_Value) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStatusType StatType = EStatusType::STATUSTYPE_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value = 0.f;

	bool operator==(const EStatusType _Type) const
	{
		return this->StatType == _Type;
	}
};

// 플레이어 스테이터스 정보
USTRUCT(BlueprintType)
struct FStatData
{
	GENERATED_BODY()

	float BaseStat = 0.f;
	float DeltaStat = 0.f;
	float TotalStat = 0.f;
};



UCLASS()
class EVERSTRIDE_API UGameDefines : public UObject
{
	GENERATED_BODY()
	
};