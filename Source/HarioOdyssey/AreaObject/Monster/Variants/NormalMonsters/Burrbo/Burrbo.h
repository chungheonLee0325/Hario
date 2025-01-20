// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HarioOdyssey/AreaObject/Monster/BaseMonster.h"
#include "Burrbo.generated.h"

UCLASS()
class HARIOODYSSEY_API ABurrbo : public ABaseMonster
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABurrbo();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
