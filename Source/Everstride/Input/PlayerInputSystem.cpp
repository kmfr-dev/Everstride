// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInputSystem.h"
#include "InputMappingContext.h"
#include "InputAction.h"

UPlayerInputSystem::UPlayerInputSystem()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext>
		MappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_DefaultMapping.IMC_DefaultMapping'"));
	if (MappingContextRef.Succeeded())
		mMappingContext = MappingContextRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>
		MoveActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Move.IA_Move'"));
	if (MoveActionRef.Succeeded())
		mMoveAction = MoveActionRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>
		RotActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_CameraRotation.IA_CameraRotation'"));
	if (RotActionRef.Succeeded())
		mCameraRotAction = RotActionRef.Object;
}

UInputMappingContext* UPlayerInputSystem::GetContext()
{
	return mMappingContext;
}

UInputAction* UPlayerInputSystem::GetMoveAction()
{
	return mMoveAction;
}

UInputAction* UPlayerInputSystem::GetCameraRotAction()
{
	return mCameraRotAction;
}
