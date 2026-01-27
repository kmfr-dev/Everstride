// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../CharacterBase.h"
#include "../../Interface/AttackInterface.h"
#include "PlayerBase.generated.h"

DECLARE_DELEGATE_TwoParams(FUpdateHPBar, float _CurHP, float _MaxHP);
DECLARE_MULTICAST_DELEGATE_OneParam(FInitUI, const FName& _TID);
DECLARE_DELEGATE_OneParam(FUsedSkill, const FName& _SkillTID);

UCLASS()
class EVERSTRIDE_API APlayerBase : public ACharacterBase
{
	GENERATED_BODY()
public:
	APlayerBase();

protected:
	// 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> mCamera = nullptr;
	
	// 스프링 암
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> mArm = nullptr;

	// 기본 Input
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UPlayerInputSystem> mInput = nullptr;

	// 무기 메쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> mWeaponMesh = nullptr;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	//TObjectPtr<class USkillComponent> mSkillComp = nullptr;
	
public:
	// UI업데이트 용 델리게이트 변수
	FUpdateHPBar OnStatUpdate;
	FInitUI OnInitUI;
	FUsedSkill OnUsedSkill;
protected:
	virtual void BeginPlay() override;
	virtual void Landed(const FHitResult& Hit) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Dead() override;

#pragma region InitSetting
private:
	virtual void DefaultSetting() override;
	virtual void InitUI() override;

	// 기본 카메라 세팅
	UFUNCTION()
	void InitCamera(class UTableSubsystem* _TableSub, const FName& _TID);

#pragma endregion InitSetting

#pragma region InputBindFunction
protected:
	// 이동 액션
	UFUNCTION()
	void MoveAction(const FInputActionValue& _InputActionValue);

	// 카메라 회전
	UFUNCTION()
	void CameraRotationAction(const FInputActionValue& _InputActionValue);

	// 달리기 액션
	UFUNCTION()
	void SprintAction(const FInputActionValue& _InputActionValue);

public:
	// 달리기 액션 종료
	UFUNCTION()
	void StopSprintAction(const FInputActionValue& _InputActionValue);

protected:
	// 공격 액션
	UFUNCTION()
	void AttackAction(const FInputActionValue& _InputActionValue);

	// 점프 액션
	UFUNCTION()
	void JumpAction(const FInputActionValue& _InputActionValue);

	UFUNCTION()
	void SlashSkillAction(const FInputActionValue& _InputActionValue);

	UFUNCTION()
	void DodgeSkillAction(const FInputActionValue& _InputActionValue);

#pragma endregion InputBindFunction

public:
	virtual void UpdateHPBar(float _CurrentHP, float _MaxHP) override;

	UFUNCTION()
	void UpdateBaseSpeed();

private:
	UFUNCTION()
	void UpdateMovementSpeed();

	UFUNCTION()
	void SkillAction(const FName& _ID);
	
	UFUNCTION()
	void SkillActionEnd();
	

public:
	UFUNCTION()
	class UCombatComponent* GetCombatComponent();

};
