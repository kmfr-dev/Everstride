// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PlayerControllerBase.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/PlayerController.h"
#include "InGamePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API AInGamePlayerController : public APlayerControllerBase, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

private:
	UPROPERTY()
	uint8 mTeamID;

public:
	virtual void BeginPlay() override;

public:
	virtual void SetGenericTeamId(const FGenericTeamId& _TeamID) override;
	virtual FGenericTeamId GetGenericTeamId() const override;

	UFUNCTION()
	void EnableBossUI(bool _Enable);

	UFUNCTION()
	void SetBossUI(class ABossMonsterBase* _BossMonster);

	UFUNCTION()
	void EnablePauseUI(bool _Enable);

private:
	// UI와 캐릭터 연동 함수
	UFUNCTION()
	void BindDelegate();
	
	UFUNCTION()
	void BindBossDelegate(class ABossMonsterBase* _BossMonster);

	UFUNCTION()
	void InitSkillSlot(const FName& _PlayerTID);

	UFUNCTION()
	void UpdateSkillUI(const FName& _SkillID);

	UFUNCTION()
	void UpdateHPBar(float _CurrentHP, float _MaxHP);


	UFUNCTION()
	void UpdateBossHP(float _NewBossHP);

};
