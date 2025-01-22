// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundManager.h"

#include "Kismet/GameplayStatics.h"

void USoundManager::PlayGlobalSound(int SoundID)
{
	auto data = SoundDataMap.Find(SoundID);
	if (data != nullptr)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), *data);
	}
}

void USoundManager::PlayPositionalSound(int SoundID, FVector Position)
{
	auto data = SoundDataMap.Find(SoundID);
	if (data != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), *data, Position);
	}
}
