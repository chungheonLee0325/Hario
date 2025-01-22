// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SoundManager.generated.h"

/**
 * 
 */
UCLASS()
class HARIOODYSSEY_API USoundManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void PlayGlobalSound(int SoundID);

	UFUNCTION()
	void PlayPositionalSound(int SoundID, FVector Position);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SoundManager")
	TMap<int, USoundBase*> SoundDataMap;
};
