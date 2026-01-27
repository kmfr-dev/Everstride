// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerInputSystem.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
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

	static ConstructorHelpers::FObjectFinder<UInputAction>
		SprintActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Sprint.IA_Sprint'"));
	if (SprintActionRef.Succeeded())
		mSprintAction = SprintActionRef.Object;
	
	static ConstructorHelpers::FObjectFinder<UInputAction>
		AttackActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Attack.IA_Attack'"));
	if (AttackActionRef.Succeeded())
		mAttackAction = AttackActionRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>
		JumpActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Jump.IA_Jump'"));
	if (JumpActionRef.Succeeded())
		mJumpAction = JumpActionRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>
		SlashSkillActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Skill_Slash.IA_Skill_Slash'"));
	if (SlashSkillActionRef.Succeeded())
		mSlashSkillAction = SlashSkillActionRef.Object;

	static ConstructorHelpers::FObjectFinder<UInputAction>
		DodgeSkillActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Skill_Dodge.IA_Skill_Dodge'"));
	if (DodgeSkillActionRef.Succeeded())
		mDodgeSkillAction = DodgeSkillActionRef.Object;
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

UInputAction* UPlayerInputSystem::GetSprintAction()
{
	return mSprintAction;
}

UInputAction* UPlayerInputSystem::GetAttackAction()
{
	return mAttackAction;
}

UInputAction* UPlayerInputSystem::GetJumpAction()
{
	return mJumpAction;
}

UInputAction* UPlayerInputSystem::GetSlashSkillAction()
{
	return mSlashSkillAction;
}

UInputAction* UPlayerInputSystem::GetDodgeSkillAction()
{
	return mDodgeSkillAction;
}
