// Fill out your copyright notice in the Description page of Project Settings.


#include "Star.h"

#include "Components/SphereComponent.h"
#include "HarioOdyssey/AreaObject/Player/Player_Mario.h"


// Sets default values
AStar::AStar()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (ItemMesh)
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> CoinMeshAsset(
			TEXT("/Script/Engine.StaticMesh'/Game/_Resource/Object/Star/estrellitaaa.estrellitaaa'"));
		if (CoinMeshAsset.Succeeded())
		{
			ItemMesh->SetStaticMesh(CoinMeshAsset.Object);
		}
		ItemMesh->SetRelativeScale3D(FVector(0.15f));
	}
	if (CollectionSphere)
	{
		CollectionSphere->SetRelativeScale3D(FVector(4.5f));
	}

	LifeTime = 12.f;
}

// Called when the game starts or when spawned
void AStar::BeginPlay()
{
	Super::BeginPlay();
}

void AStar::ApplyEffect(class APlayer_Mario* Player)
{
	Player->AddStarEffect(LifeTime);
}

// Called every frame
void AStar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
