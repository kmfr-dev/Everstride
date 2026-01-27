// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolSubsystem.h"
#include "../Interface/ObjectPoolInterface.h"

void UObjectPoolSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

}

void UObjectPoolSubsystem::InitPool(TSubclassOf<AActor> _Class, int32 _PoolSize)
{
	// 월드가 있는지 확인
	UWorld* World = GetWorld();
	if (nullptr == World || nullptr == _Class)
		return;


	// 현재 해당 클래스의 타입이 풀링되어있다면 return
	if (FPoolData* OldPool = mPoolMap.Find(_Class))
		return;

	// 생성된 액터를 임시로 담을 배열
	TArray<AActor*> SpawnedActors;
	for (int32 i = 0; i < _PoolSize; ++i)
	{
		// 액터 스폰
		AActor* PoolActor = World->SpawnActorDeferred<AActor>(_Class, FTransform(FRotator::ZeroRotator, FVector::ZeroVector),
			nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		// 액터 비활성화
		if (PoolActor)
		{
			PoolActor->SetActorHiddenInGame(true);
			PoolActor->SetActorEnableCollision(false);
			PoolActor->SetActorTickEnabled(false);

			SpawnedActors.Emplace(PoolActor);

			PoolActor->FinishSpawning(PoolActor->GetActorTransform());
		}
	}

	// 풀맵에 최종 데이터 저장
	mPoolMap.Emplace(_Class, FPoolData(_PoolSize, SpawnedActors));
}

void UObjectPoolSubsystem::ReturnToPool(AActor* _ReturnActor)
{
	if (nullptr == _ReturnActor)
		return;

	// 인자로 넘어온 액터의 클래스를 얻어온다.
	TSubclassOf<AActor> ActorClass = _ReturnActor->GetClass();

	// 풀맵 조회
	FPoolData* PoolData = mPoolMap.Find(ActorClass);
	if (nullptr == PoolData)
		return;

	// 풀 용량 체크 및 중복 처리
	bool IsAddedToPool = false;
	if (!PoolData->PoolArray.Contains(_ReturnActor))
	{
		if (PoolData->PoolArray.Num() < PoolData->PoolSize)
		{
			PoolData->PoolArray.Emplace(_ReturnActor);
			IsAddedToPool = true;
		}
	}
	// 풀에 들어가지 못하면 Destroy후 리턴
	if (!IsAddedToPool)
	{
		_ReturnActor->Destroy();
		return;
	}

	// 풀에 들어간 경우 액터 상태 초기화
	_ReturnActor->SetActorHiddenInGame(true);
	_ReturnActor->SetActorEnableCollision(false);
	_ReturnActor->SetActorTickEnabled(false);

	// 인터페이스 호출
	if (ActorClass->ImplementsInterface(UObjectPoolInterface::StaticClass()))
		IObjectPoolInterface::Execute_OnReturnToPool(_ReturnActor);
}

AActor* UObjectPoolSubsystem::GetActorFromPool(TSubclassOf<AActor> _Class)
{
	UWorld* World = GetWorld();
	if (nullptr == World || nullptr == _Class)
		return nullptr;

	FPoolData* PoolData =  mPoolMap.Find(_Class);
	if (nullptr == PoolData)
		return nullptr;

	TArray<AActor*>& PoolArray = PoolData->PoolArray;
	
	AActor* PoolActor = nullptr;
	
	// 풀에 액터가 남아있다면 사용W
	if (!PoolArray.IsEmpty())
		PoolActor = PoolArray.Pop();
	else
	{
		// 없으면 새로 Spawn하여 리턴
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		PoolActor = World->SpawnActor<AActor>(_Class, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	}

	if (false == IsValid(PoolActor))
		return nullptr;

	PoolActor->SetActorHiddenInGame(false);
	PoolActor->SetActorEnableCollision(true);
	PoolActor->SetActorTickEnabled(true);

	// 해당 액터가 풀 인터페이스를 구현했는지 확인
	if (IsValid(PoolActor) && PoolActor->GetClass()->ImplementsInterface(UObjectPoolInterface::StaticClass()))
	{
		// 구현했으면 해당함수 호출
		IObjectPoolInterface::Execute_OnSpawnFromPool(PoolActor);
	}


	return PoolActor;
}

bool UObjectPoolSubsystem::IsPooled(TSubclassOf<AActor> _Class)
{
	FPoolData* PoolData = mPoolMap.Find(_Class);
	if (PoolData)
		return true;

	return false;
}

UObjectPoolSubsystem* UObjectPoolSubsystem::Get(const UObject* _WorldContext)
{
	if (nullptr == _WorldContext)
		return nullptr;
	
	UWorld* World = _WorldContext->GetWorld();
	if (nullptr == World)
		return nullptr;

	return World->GetSubsystem<UObjectPoolSubsystem>();
}
