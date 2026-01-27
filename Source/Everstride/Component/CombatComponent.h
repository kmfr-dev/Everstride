// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class EVERSTRIDE_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<class UAttackBase> mAttack= nullptr;

	UPROPERTY()
	bool mIsCombat = false;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION()
	virtual void AttackBegin();

	UFUNCTION()
	void SetAttackData(TSubclassOf<class UAttackBase> _Class);

	UFUNCTION()
	virtual void AttackHitCheck();

public:
	UFUNCTION()
	void SetCombat(bool _IsCombat) { mIsCombat = _IsCombat; }
	UFUNCTION()
	bool IsCombat() const { return mIsCombat; };
		
};
