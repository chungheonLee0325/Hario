// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "ChainComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HARIOODYSSEY_API UChainComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UChainComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void UpdateChainPosition(FVector Start, FVector End);
	
	UPROPERTY(EditAnywhere, Category = "Chain Settings")
	USceneComponent* RootScence;

private:
	// 설정값들
	UPROPERTY(EditAnywhere, Category = "Chain Settings")
	UStaticMesh* ChainLinkMesh;

	UPROPERTY(EditAnywhere, Category = "Chain Settings")
	TArray<UStaticMeshComponent*> ChainLinks;


	
	const int ChainNum = 7;
};
