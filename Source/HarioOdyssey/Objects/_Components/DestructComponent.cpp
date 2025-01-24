// Fill out your copyright notice in the Description page of Project Settings.


#include "DestructComponent.h"

#include "GeometryCollection/GeometryCollectionEngineSizeSpecificUtility.h"


// Sets default values for this component's properties
UDestructComponent::UDestructComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UDestructComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDestructComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UDestructComponent::ApplyDestruction(FVector ActorLocation)
{
	OnDestruction.Broadcast(ActorLocation);
}



