// Fill out your copyright notice in the Description page of Project Settings.


#include "HarioGameMode.h"

#include "HarioGameInstance.h"
#include "Components/AudioComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

AHarioGameMode::AHarioGameMode()
{
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMAudioComponennt"));
	AudioComponent->bAutoDestroy = false;
	AudioComponent->bAutoManageAttachment = false;
	SoundDataMap.Empty();
}

void AHarioGameMode::BeginPlay()
{
	Super::BeginPlay();

	//PlayerDied 델리게이트를 게임 모드의 PlayerDied 함수에 바인딩.
	if (!OnPlayerDied.IsBound())
	{
		OnPlayerDied.AddDynamic(this, &AHarioGameMode::PlayerDied);
	}
	auto gameInstance = Cast<UHarioGameInstance>(GetGameInstance());
	if (nullptr == gameInstance)
	{
		return;
	}
	SoundDataMap = gameInstance->SoundDataMap;

	PlayBGM(1001, true);
}


void AHarioGameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);
}

void AHarioGameMode::PlayerDied(ACharacter* Character)
{
	//캐릭터의 플레이어 컨트롤러에 대한 레퍼런스 구하기
	AController* CharacterController = Character->GetController();
	RestartPlayer(CharacterController);
}

void AHarioGameMode::PlayGlobalSound(int SoundID)
{
	auto data = SoundDataMap.Find(SoundID);
	if (data != nullptr)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), *data);
	}
}

void AHarioGameMode::PlayPositionalSound(int SoundID, FVector Position)
{
	auto data = SoundDataMap.Find(SoundID);
	if (data != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), *data, Position);
	}
}

void AHarioGameMode::PlayBGM(int SoundID, bool bLoop)
{
	auto data = SoundDataMap.Find(SoundID);
	// 이전 BGM 정지
	StopBGM();

	// 새로운 BGM 설정
	if (data)
	{
		CurrentBGM = *data;
		AudioComponent->SetSound(CurrentBGM);
		AudioComponent->bAlwaysPlay = bLoop;
		AudioComponent->Play();
	}
}

void AHarioGameMode::StopBGM()
{
	// 현재 재생 중인 BGM 정지
	if (AudioComponent)
	{
		AudioComponent->Stop();
		CurrentBGM = nullptr;
	}
}

void AHarioGameMode::SetBGMVolume(float Volume)
{
	// BGM 볼륨 조절 (0.0f ~ 1.0f)
	if (AudioComponent)
	{
		AudioComponent->SetVolumeMultiplier(FMath::Clamp(Volume, 0.0f, 1.0f));
	}
}
