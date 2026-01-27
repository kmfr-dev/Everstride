// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePlayerController.h"
#include "../Actors/Player/PlayerBase.h"
#include "../Component/StatComponent.h"
#include "../Subsystem/UISubsystem.h"
#include "../UI/InGame/InGameMainWidget.h"
#include "../Actors/Monster/BossMonsterBase.h"

void AInGamePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 델리게이트 바인딩
	BindDelegate();

	// 인게임 레벨이므로 Input모드를 Game으로 적용
	FInputModeGameOnly InputMode;
	SetShowMouseCursor(false);
	SetInputMode(InputMode);
}

void AInGamePlayerController::SetGenericTeamId(const FGenericTeamId& _TeamID)
{
	mTeamID = _TeamID;
}

FGenericTeamId AInGamePlayerController::GetGenericTeamId() const
{
	return mTeamID;
}

void AInGamePlayerController::BindDelegate()
{
	// 내 자신의 캐릭터를 얻어온다.
	APlayerBase* PlayerPtr = GetPawn<APlayerBase>();
	
	// 유효할 때만
	if (IsValid(PlayerPtr))
	{
		//  델리게이트에 함수 바인딩
		PlayerPtr->OnStatUpdate.BindUObject(this, &AInGamePlayerController::UpdateHPBar);
		PlayerPtr->OnInitUI.AddUObject(this, &AInGamePlayerController::InitSkillSlot);
		PlayerPtr->OnUsedSkill.BindUObject(this, &AInGamePlayerController::UpdateSkillUI);
	}
}

void AInGamePlayerController::BindBossDelegate(ABossMonsterBase* _Boss)
{
	if (nullptr == _Boss)
		return;

	_Boss->OnChangedHP.BindUObject(this, &AInGamePlayerController::UpdateBossHP);
}

void AInGamePlayerController::InitSkillSlot(const FName& _PlayerTID)
{
	UUISubsystem* UISub = UUISubsystem::Get(this);
	if (nullptr == UISub)
		return;

	// 메인 위젯을 얻어온다.
	UInGameMainWidget* MainWidget = Cast<UInGameMainWidget>(UISub->GetMainWidget());
	if (nullptr == MainWidget)
		return;

	MainWidget->SetSkillSlotUI(_PlayerTID);
}

void AInGamePlayerController::UpdateSkillUI(const FName& _SkillID)
{
	UUISubsystem* UISub = UUISubsystem::Get(this);
	if (nullptr == UISub)
		return;

	// 메인 위젯을 얻어온다.
	UInGameMainWidget* MainWidget = Cast<UInGameMainWidget>(UISub->GetMainWidget());
	if (nullptr == MainWidget)
		return;

	MainWidget ->UpdateSkillSlotUI(_SkillID);
}

void AInGamePlayerController::UpdateHPBar(float _CurrentHP, float _MaxHP)
{
	UUISubsystem* UISub = UUISubsystem::Get(this);
	if (nullptr == UISub)
		return;

	// 메인 위젯을 얻어온다.
	UInGameMainWidget* MainWidget = Cast<UInGameMainWidget>(UISub->GetMainWidget());
	if (nullptr == MainWidget)
		return;
	
	const float NewPercent = _CurrentHP / _MaxHP;

	MainWidget->SetMaxHPText(_MaxHP);
	MainWidget->SetCurHPBarText(_CurrentHP);
	MainWidget->SetHPBarPercent(NewPercent);
}

void AInGamePlayerController::EnableBossUI(bool _Enable)
{
	UUISubsystem* UISub = UUISubsystem::Get(this);
	if (nullptr == UISub)
		return;

	// 메인 위젯을 얻어온다.
	UInGameMainWidget* MainWidget = Cast<UInGameMainWidget>(UISub->GetMainWidget());
	if (nullptr == MainWidget)
		return;

	MainWidget->SetEnableBossUI(_Enable);
}

void AInGamePlayerController::SetBossUI(ABossMonsterBase* _BossMonster)
{
	if (nullptr == _BossMonster)
		return;

	BindBossDelegate(_BossMonster);

	UUISubsystem* UISub = UUISubsystem::Get(this);
	if (nullptr == UISub)
		return;

	// 메인 위젯을 얻어온다.
	UInGameMainWidget* MainWidget = Cast<UInGameMainWidget>(UISub->GetMainWidget());
	if (nullptr == MainWidget)
		return;
	
	UStatComponent* StatComp = _BossMonster->GetStatComponent();
	if (nullptr == StatComp)
		return;


	float CurHP = StatComp->GetTotalStatValue(EStatusType::STATTYPE_HP);
	const float MaxHP = StatComp->GetTotalStatValue(EStatusType::STATTYPE_MAXHP);

	CurHP /= MaxHP;

	MainWidget->SetBossName(_BossMonster->GetMonsterName().ToString());
	MainWidget->SetBossHPBarPercent(CurHP);
}

void AInGamePlayerController::EnablePauseUI(bool _Enable)
{
	UUISubsystem* UISub = UUISubsystem::Get(this);
	if (nullptr == UISub)
		return;

	// 메인 위젯을 얻어온다.
	UInGameMainWidget* MainWidget = Cast<UInGameMainWidget>(UISub->GetMainWidget());
	if (nullptr == MainWidget)
		return;

}

void AInGamePlayerController::UpdateBossHP(float _NewBossHP)
{
	UUISubsystem* UISub = UUISubsystem::Get(this);
	if (nullptr == UISub)
		return;

	// 메인 위젯을 얻어온다.
	UInGameMainWidget* MainWidget = Cast<UInGameMainWidget>(UISub->GetMainWidget());
	if (nullptr == MainWidget)
		return;

	// HP업데이트
	MainWidget->SetBossHPBarPercent(_NewBossHP);
}
