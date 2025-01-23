// Fill out your copyright notice in the Description page of Project Settings.


#include "HarioGameMode.h"

#include "GameFramework/Character.h"

void AHarioGameMode::BeginPlay()
{
	Super::BeginPlay();

	//PlayerDied 델리게이트를 게임 모드의 PlayerDied 함수에 바인딩.
	if (!OnPlayerDied.IsBound())
	{
		OnPlayerDied.AddDynamic(this, &AHarioGameMode::PlayerDied);
	}
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