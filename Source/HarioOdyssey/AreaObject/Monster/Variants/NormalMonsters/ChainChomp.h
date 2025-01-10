// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HarioOdyssey/AreaObject/Monster/Monster.h"
#include "ChainChomp.generated.h"

class USphereComponent;

UCLASS()
class HARIOODYSSEY_API AChainChomp : public AMonster
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AChainChomp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere)
	USceneComponent* ChainChompRoot;

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* ChainChompMesh;

	UPROPERTY(EditAnywhere)
	USphereComponent* ChainChompSphere;
};
