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


UENUM(BlueprintType)
enum class EStatusType : uint8
{
	STATUSTYPE_NONE,


};

// 플레이어 스테이터스 정보
USTRUCT(BlueprintType)
struct FStatusData
{
	GENERATED_BODY()

public:
	FStatusData() {}
	FStatusData(EStatusType _Type, float _Value) : Type(_Type), Value(_Value) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStatusType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Value;

	bool operator==(const EStatusType _Type) const
	{
		return this->Type == _Type;
	}
};



UCLASS()
class EVERSTRIDE_API UGameDefines : public UObject
{
	GENERATED_BODY()
	
};