// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HarioOdyssey/Items/BaseItem.h"
#include "Coin.generated.h"

UCLASS()
class HARIOODYSSEY_API ACoin : public ABaseItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ACoin();
	
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ApplyEffect(class APlayer_Mario* Player) override;

	UPROPERTY(EditAnywhere, Category = "Coin")
	int32 m_CoinValue;

private:
	FRotator CoinRotator = FRotator(0, 60.f , 0);
};
