// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HarioOdyssey/ResourceManager/HarioGameType.h"
#include "HarioGameInstance.generated.h"

class USoundManager;
/**
 * 
 */
UCLASS()
class HARIOODYSSEY_API UHarioGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	void Init() override;

public:
	FAreaObjectData* GetDataAreaObject(int AreaObjectID);
	
	TMap<int32, FAreaObjectData> dt_AreaObject;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundManager* SoundManager;
};
