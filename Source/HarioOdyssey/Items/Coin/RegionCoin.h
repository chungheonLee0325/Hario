// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HarioOdyssey/Items/BaseItem.h"
#include "RegionCoin.generated.h"

UCLASS()
class HARIOODYSSEY_API ARegionCoin : public ABaseItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARegionCoin();

	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ApplyEffect(class APlayer_Mario* Player) override;

	//코인 누적
	UPROPERTY(EditAnywhere, Category = "Coin")
	int32 m_CoinValue;
};
