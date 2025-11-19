// Fill out your copyright notice in the Description page of Project Settings.


#include "UISubsystem.h"
#include "TableSubsystem.h"
#include "../EverstrideGameInstance.h"


void UUISubsystem::SetMainWidget()
{
	UWorld* World = GetWorld();
	if (nullptr == World)
		return;

	UEverstrideGameInstance* GameInst = World->GetGameInstance<UEverstrideGameInstance>();
	if (nullptr == GameInst)
		return;

	// 게임 인스턴스에서 현재 레벨 타입을 얻어온다.
	ELevelType LevelType = GameInst->GetLevelType();

	// 테이블 서브시스템을 얻어온다.
	UTableSubsystem* TableSub = UTableSubsystem::Get(this);
	if (nullptr == TableSub)
		return;

	FName MainWidgetName;

	// 레벨 타입에 따라 메인위젯 이름 설정
	switch (LevelType)
	{
	case ELevelType::LEVEL_TYPE_TITLE:
		MainWidgetName = UTableData::MainWidgetName::TITLE;
		break;
	case ELevelType::LEVEL_TYPE_INGAME:
		MainWidgetName = UTableData::MainWidgetName::INGAME;
		break;
	case ELevelType::LEVEL_TYPE_NONE:
	default:
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("레벨 타입이 유효하지 않습니다."));
		return;
	}

	// 메인위젯 이름에 따라 메인위젯 테이블에서 행을 읽어온다.
	FMainWidgetTableRow* FoundRow = TableSub->FindTableRow<FMainWidgetTableRow>
		(UTableData::TableName::MAINWIDGET, MainWidgetName);

	if (nullptr == FoundRow)
		return;

	// 해당 행의 메인위젯 클래스로 위젯 생성
	mCurrentMainWidget = CreateWidget<UUserWidget>(World, FoundRow->MainWidgetClass);
	// 생성한 위젯이 유효할 때만 뷰포트에 추가한다.
	if (true == IsValid(mCurrentMainWidget))
	{
		mCurrentMainWidget->AddToViewport();
	}
}

void UUISubsystem::ClearMainWidget()
{
	// 메인위젯 제거
	if (true == IsValid(mCurrentMainWidget))
	{
		mCurrentMainWidget->RemoveFromParent();
	}
}

UUISubsystem* UUISubsystem::Get(const UObject* _WorldContext)
{
	if (nullptr == _WorldContext)
		return nullptr;

	UGameInstance* GameInst = UGameplayStatics::GetGameInstance(_WorldContext);
	if (nullptr == GameInst)
		return nullptr;

	return GameInst->GetSubsystem<UUISubsystem>();
}
