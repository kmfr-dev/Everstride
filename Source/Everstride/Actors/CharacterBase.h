
#pragma once

#include "../Common/GameDefines.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "../Interface/AttackInterface.h"
#include "CharacterBase.generated.h"

UCLASS()
class EVERSTRIDE_API ACharacterBase : public ACharacter, public IGenericTeamAgentInterface, public IAttackInterface
{
	GENERATED_BODY()

public:
	ACharacterBase();

#pragma region Variable
protected:
	// 스켈레탈 메쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> mSkeletalMesh;

	// 기본 플레이어 애님 인스턴스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UAnimInstanceBase> mAnim = nullptr;

	// 능력치 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UStatComponent> mStatComponent = nullptr;

	// 전투 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UCombatComponent> mCombatComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class USkillComponent> mSkillComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FSkillMappingData> mSkillMappingData;

	UPROPERTY()
	FName mTID;

	UPROPERTY()
	TObjectPtr<class UCharacterActionTag> mTagData = nullptr;

	UPROPERTY()
	bool mDead = false;
#pragma endregion Variable

protected:
	virtual void BeginPlay() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator, AActor* DamageCauser) override;
public:	
	virtual void Tick(float DeltaTime) override;

#pragma region InitSetting
protected:
	// 캐릭터의 기본값 세팅
	UFUNCTION()
	virtual void DefaultSetting();

	UFUNCTION()
	virtual void InitUI();

	// 기본 메쉬 세팅
	UFUNCTION()
	void InitMesh(class UTableSubsystem* _TableSub, const FName& _TID);

	// 기본 애님 세팅
	UFUNCTION()
	void InitAnimInstance(class UTableSubsystem* _TableSub, const FName& _TID);

	// 기본 공격 세팅
	UFUNCTION()
	void InitAttack(TSubclassOf<class UAttackBase> _Class);

	// 태그 세팅
	UFUNCTION()
	void InitTag(class UTableSubsystem* _TableSub, const FName& _TID);

	UFUNCTION()
	void InitCollision(class UTableSubsystem* _TableSub, const FName& _TID);

	UFUNCTION()
	void InitSkill(class UTableSubsystem* _TableSub, const FName& _TID);

	UFUNCTION()
	void InitStatus(const FName& _TID);

	// 캐릭터 기본 무브먼트 세팅
	UFUNCTION()
	void InitMovement();

	

#pragma endregion InitSetting

#pragma region Interface Function Override 
protected:
	UFUNCTION()
	virtual void AttackHitCheck() override;


#pragma endregion

#pragma region State
	virtual void Dead();
	virtual void Hit(AActor* _HitActor);
#pragma endregion

#pragma region UIUpdate Function
	UFUNCTION()
	virtual void UpdateHPBar(float _CurrentHP, float _MaxHP);

#pragma endregion

#pragma region Get & Set
public:
	UFUNCTION()
	class USkillComponent* GetSkillComponent();

	UFUNCTION(BlueprintCallable)
	class UStatComponent* GetStatComponent();

	FSkillMappingData* GetSkillMappingData(const FGameplayTag& _Tag);

	UFUNCTION()
	void SetCharacterTID(const FName& _TID) { mTID = _TID; }
	UFUNCTION(BlueprintCallable)
	const FName& GetCharacterTID() const { return mTID; }

	UFUNCTION()
	bool IsDead() const { return mDead; }

#pragma endregion Get & Set
};
