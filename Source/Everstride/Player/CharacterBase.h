#pragma once

#include "../Common/GameDefines.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS()
class EVERSTRIDE_API ACharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACharacterBase();

#pragma region Variable
protected:
	// 스프링 암
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USpringArmComponent> mArm = nullptr;

	// 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UCameraComponent> mCamera = nullptr;

	//스켈레탈 메쉬
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> mSkeletalMesh = nullptr;

	// 기본 Input
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UPlayerInputSystem> mInput = nullptr;

	// 기본 플레이어 애님 인스턴스
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UAnimInstanceBase> mAnim = nullptr;

	// 능력치 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UStatComponent> mStatComponent = nullptr;

#pragma endregion Variable

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region InitSetting
private:
	// 캐릭터의 기본값 세팅
	UFUNCTION()
	void DefaultSetting();

	// 캐릭터 기본 무브먼트 세팅
	UFUNCTION()
	void InitMovement();

	// 기본 메쉬 세팅
	UFUNCTION()
	void InitMesh();

	// 기본 카메라 세팅
	UFUNCTION()
	void InitCamera();
#pragma endregion InitSetting

#pragma region InputBindFunction
protected:
	// 이동 액션
	UFUNCTION()
	void MoveAction(const FInputActionValue& _InputActionValue);

	// 카메라 회전
	UFUNCTION()
	void CameraRotationAction(const FInputActionValue& _InputActionValue);
#pragma endregion InputBindFunction

#pragma region Get & Set
public:
	UFUNCTION()
	class UStatComponent* GetStatComponent();

#pragma endregion Get & Set

};
