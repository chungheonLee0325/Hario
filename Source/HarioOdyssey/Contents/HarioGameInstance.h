// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HarioOdyssey/ResourceManager/Hario_Struct.h"
#include "HarioGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class HARIOODYSSEY_API UHarioGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:
	virtual void Init() override;

public:
	TMap<int32, FAreaObjectData> d_AreaObject;
};
