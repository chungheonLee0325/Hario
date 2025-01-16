// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HarioOdyssey/AreaObject/Monster/AI/Base/BaseAiFSM.h"
#include "AiChainChomp.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HARIOODYSSEY_API UAiChainChomp : public UBaseAiFSM
{
	GENERATED_BODY()

public:
	UAiChainChomp();
	virtual void InitStatePool() override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
							   FActorComponentTickFunction* ThisTickFunction) override;


private:
	UPROPERTY()
	FVector m_OriginalPosition;
};
