// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseWidget.h"
#include "../Common/ButtonBase.h"
#include "Kismet/GameplayStatics.h"
#include "../../Input/InGamePlayerController.h"

void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	mResume->SetButtonText("Resume");
	mResume->OnClickedButton.AddDynamic(this, &UPauseWidget::OnClickedResumeBtn);

	mExit->SetButtonText("Exit");
	mExit->OnClickedButton.AddDynamic(this, &UPauseWidget::OnClickedResumeBtn);
}

void UPauseWidget::OnClickedResumeBtn()
{
	AInGamePlayerController* PlayerControllerPtr = GetOwningPlayer<AInGamePlayerController>();
	if (nullptr == PlayerControllerPtr)
		return;

	UGameplayStatics::SetGamePaused(GetWorld(), false);

}

void UPauseWidget::OnClickedExitBtn()
{
	UGameplayStatics::OpenLevel(GetWorld(), "/Game/Level/Title");
}


