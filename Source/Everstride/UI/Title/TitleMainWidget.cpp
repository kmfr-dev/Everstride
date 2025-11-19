// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleMainWidget.h"
#include "../Common/ButtonBase.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UTitleMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼 기본 세팅

	if (IsValid(mGameStart))
	{
		mGameStart->SetButtonText("Game Start");
		mGameStart->OnClickedButton.AddDynamic(this, &UTitleMainWidget::OnClickedGameStartBtn);
	}

	if (IsValid(mExit))
	{
		mExit->SetButtonText("Exit");
		mExit->OnClickedButton.AddDynamic(this, &UTitleMainWidget::OnClickedExitBtn);
	}
}

void UTitleMainWidget::OnClickedGameStartBtn()
{
	// 인게임 레벨로 전환
	//UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/Levels/01_Title/Title.Title'"));
}

void UTitleMainWidget::OnClickedExitBtn()
{
	// 게임 종료
	APlayerController* PlayerControllerPtr = GetOwningPlayer();
	if (PlayerControllerPtr)
		UKismetSystemLibrary::QuitGame(GetWorld(), PlayerControllerPtr, EQuitPreference::Quit, true);
}
