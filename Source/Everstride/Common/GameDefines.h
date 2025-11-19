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




UCLASS()
class EVERSTRIDE_API UGameDefines : public UObject
{
	GENERATED_BODY()
	
};