// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HarioOdyssey/Items/BaseItem.h"
#include "Star.generated.h"

UCLASS()
class HARIOODYSSEY_API AStar : public ABaseItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AStar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ApplyEffect(class APlayer_Mario* Player) override;

	// Item Setting
	UPROPERTY(EditAnywhere, Category = "Item Setting")
	float LifeTime;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
