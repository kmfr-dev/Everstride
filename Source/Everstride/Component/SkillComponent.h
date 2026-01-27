// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Common/GameDefines.h"
#include "Components/ActorComponent.h"
#include "SkillComponent.generated.h"

DECLARE_DELEGATE(FOnSkillFinished)

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EVERSTRIDE_API USkillComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillComponent();

protected:
	UPROPERTY()
	TArray<FSkillData> mSkillDatas;

	UPROPERTY()
	FName mCastSkill = NAME_None;

	ASkillBase* mCastingSkillActor = nullptr;

	FOnSkillFinished OnSkillFinished;

	FOnMontageEnded OnSkillActionEnd;
protected:
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 스킬 세팅 함수
	UFUNCTION()
	void SetSkillData(const FName& _ID);
	
	// 스킬 선택 함수
	UFUNCTION()
	bool ChooseSkill(const FName& _ID);

	// 스킬 액션 함수
	UFUNCTION()
	bool SkillAction(const FName& _ID);

	UFUNCTION()
	void SkillActionPreStart();

	// 실제 스킬 사용 함수
	UFUNCTION()
	void SkillActionStart();

	// 스킬 사용 종료 시
	UFUNCTION()
	void SkillEnd();
	
	// 스킬 사용 몽타주 종료시
	UFUNCTION()
	void SkillActionEnd(UAnimMontage* _Montage, bool _Cancel);

	// 사용중인 스킬 액터 얻어오는 함수
	UFUNCTION()
	ASkillBase* GetCastingSkillActor();

	void SetSkillFinished(const FOnSkillFinished& _InSkillFinished);

	UFUNCTION()
	const TArray<FSkillData>& GetSkillDatas();

	// 현재 시전중인 스킬의 ID반환하는 함수
	UFUNCTION()
	const FName& GetCastingSkill();
private:
	UFUNCTION()
	bool IsSameSkill(const FName& _ID);

	// 스킬 쿨타임 재설정함수
	UFUNCTION()
	void RefreshSkillCoolDown(const FName& _ID);

	// 관리하고있는 스킬데이터에서 스킬 찾는 함수
	FSkillData* FindSkill(const FName& _ID);

	// 현재 시전중인 스킬 비워주는 함수
	UFUNCTION()
	void ClearCastSkill();

};
