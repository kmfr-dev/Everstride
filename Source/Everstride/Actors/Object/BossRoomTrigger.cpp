// Fill out your copyright notice in the Description page of Project Settings.


#include "BossRoomTrigger.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"

#include "LevelSequence.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "../../Input/InGamePlayerController.h"

#include "../../Subsystem/UISubsystem.h"
#include "../Monster/BossMonsterBase.h"
#include "../Monster/Controller/AIControllerBossMonster.h"
#include "../Player/PlayerBase.h"

// Sets default values
ABossRoomTrigger::ABossRoomTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mCollision = CreateDefaultSubobject<UBoxComponent>("Collision");
}

// Called when the game starts or when spawned
void ABossRoomTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	if(IsValid(mCollision))
		mCollision->OnComponentBeginOverlap.AddDynamic(this, &ABossRoomTrigger::OnTriggerBeginOverlap);

   
}

void ABossRoomTrigger::OnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (!mLevelSequenceAsset) 
        return;

    FMovieSceneSequencePlaybackSettings Settings;
    Settings.bAutoPlay = false;
    Settings.LoopCount.Value = 0;

    ALevelSequenceActor* SequenceActor = nullptr;

    ULevelSequencePlayer* SequencePlayer =
        ULevelSequencePlayer::CreateLevelSequencePlayer(
            GetWorld(),
            mLevelSequenceAsset,
            Settings,
            SequenceActor
        );

    if (!SequencePlayer || !SequenceActor)
        return;

    ABossMonsterBase* BossMonster = Cast<ABossMonsterBase>(UGameplayStatics::GetActorOfClass(GetWorld(), ABossMonsterBase::StaticClass()));
    if (nullptr == BossMonster)
        return;

    mBoss = BossMonster;

    APlayerBase* Player = Cast<APlayerBase>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerBase::StaticClass()));
    if (nullptr == Player)
        return;

    mPlayer = Player;

    // 런타임 바인딩
    SequenceActor->SetBindingByTag(FName("Boss"), { BossMonster }, false);
    

    SequencePlayer->OnPlay.AddDynamic(this, & ABossRoomTrigger::OnSequenceStart);
    SequencePlayer->OnFinished.AddDynamic(this, &ABossRoomTrigger::OnSequenceEnd);

    SequencePlayer->Play();

    mCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABossRoomTrigger::OnSequenceStart()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (nullptr == PC)
        return;

    APlayerBase* PCPawn = PC->GetPawn<APlayerBase>();
    if (nullptr == PCPawn)
        return;

    PCPawn->UpdateBaseSpeed();
    
    PCPawn->DisableInput(PC);

    APlayerCameraManager* CamMgr = PC->PlayerCameraManager;
    if (nullptr == CamMgr)
        return;

    CamMgr->StartCameraFade(0.f, 1.f, 0.3f, FLinearColor::Black, false, true);
    
    UUISubsystem* UISub = UUISubsystem::Get(this);
    if (nullptr == UISub)
        return;
    
    UISub->HideMainWidget();
}

void ABossRoomTrigger::OnSequenceEnd()
{
    AInGamePlayerController* PC = Cast<AInGamePlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
    if (nullptr == PC)
        return;

    APawn* PCPawn = PC->GetPawn();
    if (nullptr == PCPawn)
        return;

    // 막아놨던 입력을 다시 활성화
    PCPawn->EnableInput(PC);

    APlayerCameraManager* CamMgr = PC->PlayerCameraManager;
    if (nullptr == CamMgr)
        return;

    // 카메라 페이드 아웃 
    CamMgr->StartCameraFade(1.f, 0.f, 0.3f, FLinearColor::Black, false, false);
    
    UUISubsystem* UISub = UUISubsystem::Get(this);
    if (nullptr == UISub)
        return;
    
    // 다시 메인 위젯 활성화 
    UISub->ShowMainWidget();
    // 보스 UI 활성화
    PC->EnableBossUI(true);
    PC->SetBossUI(mBoss);

    // 타겟 설정 및 비헤이비어 트리 시작
    BossCombatReady();

    Destroy();
}

void ABossRoomTrigger::BossCombatReady()
{
    if (nullptr == mBoss || nullptr == mPlayer)
        return;

    AAIControllerBossMonster* AIOwner = mBoss->GetController<AAIControllerBossMonster>();
    if (nullptr == AIOwner)
        return;

    // 타겟 지정및 트리 시작
    AIOwner->RunTree();
    mBoss->SetBlackBoardDefaultHP();
    AIOwner->SetTarget(mPlayer);

}
// Called every frame
void ABossRoomTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

