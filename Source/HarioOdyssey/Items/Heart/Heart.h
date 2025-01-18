// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HarioOdyssey/Items/BaseItem.h"
#include "Heart.generated.h"

UCLASS()
class HARIOODYSSEY_API AHeart : public ABaseItem
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHeart();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void ApplyEffect(APlayer_Mario* Player) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Heart 회복수치
UPROPERTY(EditAnywhere)
	float m_HeartValue;

};
