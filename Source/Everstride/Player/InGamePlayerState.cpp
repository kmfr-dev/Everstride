// Fill out your copyright notice in the Description page of Project Settings.


#include "InGamePlayerState.h"
#include "../Subsystem/TableSubsystem.h"

void AInGamePlayerState::BeginPlay()
{
	Super::BeginPlay();

	//기본 테이블 서브시스템을 불러옴
	UTableSubsystem* TableSub = UTableSubsystem::Get(this);
	if (nullptr == TableSub)
		return;

	// DefineTable에 정의된 기본 플레이어 행을 얻어온다.
	FDefineTableRow* DefineRow = TableSub->FindTableRow<FDefineTableRow>(UTableData::TableName::DEFINE, UTableData::DefineName::DEFAULT_PLAYER);
	if (nullptr == DefineRow)
		return;

	// 플레이어 TID지정
	mPlayerTID = FName(DefineRow->StrVal);
}
