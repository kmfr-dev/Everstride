
#pragma once

#include "EngineMinimal.h"
#include "Engine.h"
#include "Engine/DamageEvents.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Object.h"
#include "NativeGameplayTags.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "GameDefines.generated.h"

#define POOLSIZE 100

namespace EverstrideGamePlayTags
{
	EVERSTRIDE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AnimMontage_Monster_Attack);
	EVERSTRIDE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AnimMontage_Monster_Hit);
	EVERSTRIDE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AnimMontage_Monster_Death);
	
	EVERSTRIDE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AnimMontage_BossMonster_RoomTrigger);
	EVERSTRIDE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AnimMontage_BossMonster_Enrage);


	EVERSTRIDE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AnimMontage_Player_Attack);
	EVERSTRIDE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AnimMontage_Player_Hit);
	EVERSTRIDE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_AnimMontage_Player_Death);

	EVERSTRIDE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Skill_Player_Slash);
	EVERSTRIDE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Skill_Player_Dodge);

	EVERSTRIDE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Skill_BossMonster_ThrowRock);
	EVERSTRIDE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Skill_BossMonster_Roar);
	EVERSTRIDE_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(TAG_Skill_BossMonster_GrondLand);
};

UENUM()
enum class ELevelType : uint8
{
	LEVEL_TYPE_NONE,
	LEVEL_TYPE_TITLE,
	LEVEL_TYPE_INGAME
};

UENUM()
enum class EMonsterType : uint8
{
	MONSTER_TYPE_NONE,
	MONSTER_TYPE_NEARING,
	MONSTER_TYPE_RANGE,
	MONSTER_TYPE_BOSS,
};

UENUM()
enum class EStatBarType : uint8
{
	STATBAR_TYPE_NONE,
	STATBAR_TYPE_HP,
	STATBAR_TYPE_STAMINA,
};

UENUM()
enum class EUIStatBarType : uint8
{
	UISTATBAR_TYPE_NONE,
	UISTATBAR_TYPE_MAX,
	UISTATBAR_TYPE_CURRENT,
};


// 스탯 타입
UENUM(BlueprintType)
enum class EStatusType : uint8
{
	STATUSTYPE_NONE				UMETA(DisplayName = "None"),
	STATTYPE_SPEED				UMETA(DisplayName = "MoveSpeed"),
	STATTYPE_HP					UMETA(DisplayName = "HP"),
	STATTYPE_MAXHP				UMETA(DisplayName = "MaxHP"),
	STATTYPE_DEFENCE				UMETA(DisplayName = "Defence"),
	STATTYPE_STAMINA			UMETA(DisplayName = "Stamina"),
	STATTYPE_MAXSTAMINA				UMETA(DisplayName = "MaxStamina"),
	STATTYPE_RUNRATIO			UMETA(DisplayName = "RunRatio"),
	STATTYPE_ATTACK				UMETA(DisplayName = "Attack"),
	STATTYPE_ATTACKRANGE				UMETA(DisplayName = "AttackRange"),
	STATTYPE_ATTACKRADIUS				UMETA(DisplayName = "AttackRadius"),
};

// 스탯 카테고리, 기본인지, 추가된 값인지, 토탈값인지
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

USTRUCT(BlueprintType)
struct FProjectileData
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed = 0.f;      

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LifeTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GravityScale = 0.f;

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

UENUM(BlueprintType)
enum class ESkillActionType : uint8
{
	SKILLACTION_TYPE_NONE,
	SKILLACTION_TYPE_INSTANT,
	SKILLACTION_TYPE_NOTIFY,
};

UENUM(BlueprintType)
enum class ESkillPoolingType : uint8
{
	SKILLPOLLING_TYPE_NONE,     
	SKILLPOLLING_TYPE_POOLED,     
	SKILLPOLLING_TYPE_NONPOOLED,     
};

USTRUCT(BlueprintType)
struct FSkillData
{
	GENERATED_BODY()

	FSkillData() {}
	FSkillData(const FSkillData& _Skill)
	{
		TID = _Skill.TID;
		CurCoolDown = _Skill.CurCoolDown;
		CoolDown = _Skill.CoolDown;
	}

	FSkillData(const FName& _TID, float _CoolDown)
	{
		TID = _TID;
		CoolDown = _CoolDown;
		CurCoolDown = _CoolDown;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TID;

	//지금 몇초인지?
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurCoolDown = 0.f;

	//전체 쿨
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CoolDown = 0.f;

	bool operator == (const FName& _TID)
	{
		return this->TID == _TID;
	}
};

USTRUCT(BlueprintType)
struct FSkillMappingData
{
	GENERATED_BODY()

	FSkillMappingData(){}
	FSkillMappingData(const FGameplayTag& _Tag, const FName& _ID)
	{
		SkillTag = _Tag;
		SkillID = _ID;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTag SkillTag = FGameplayTag::EmptyTag;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName SkillID = NAME_None;

	bool operator == (const FGameplayTag& _Tag)
	{
		return this->SkillTag == _Tag;
	}
};

USTRUCT(BlueprintType)
struct FSkillRateData
{
	GENERATED_BODY()

	// 스킬에 영향을 줄 능력치 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStatusType Type = EStatusType::STATUSTYPE_NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Rate = 0.f;
};

USTRUCT(BlueprintType)
struct FPoolData
{
	GENERATED_BODY()

	FPoolData() {}
	FPoolData(int32 _Size, const TArray<AActor*>& _Arr) : PoolSize(_Size), PoolArray(_Arr) {}

	UPROPERTY()
	int32 PoolSize = 0;

	UPROPERTY()
	TArray<AActor*> PoolArray;
};


UCLASS()
class EVERSTRIDE_API UGameDefines : public UObject
{
	GENERATED_BODY()
	
};