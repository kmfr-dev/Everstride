// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "UIView.generated.h"

/**
 * 
 */
UCLASS()
class EVERSTRIDE_API UUIView : public UObject
{
	GENERATED_BODY()
	
protected:
	FName mTID = NAME_None;

public:
	void SetTID(const FName& _TID) { mTID = _TID; }
	const FName& GetTID() { return mTID; }
};


UCLASS()
class EVERSTRIDE_API USkillView : public UUIView
{
	GENERATED_BODY()

protected:
	UTexture2D* mSkillIcon = nullptr;

public:
	void SetSkillIcon(UTexture2D* _SkillIcon) { mSkillIcon = _SkillIcon; }
	UTexture2D* GetSkillIcon() { return mSkillIcon; }
};