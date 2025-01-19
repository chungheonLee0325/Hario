// Fill out your copyright notice in the Description page of Project Settings.


#include "RootAnchor.h"

#include "MovieSceneSequenceID.h"


// Sets default values
ARootAnchor::ARootAnchor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	RootComponent = Mesh;
	Mesh->SetRelativeScale3D(FVector(0.3f));
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/_Resource/Monster/ChainChomp/ST_Peg.ST_Peg'"));
	if (tempMesh.Succeeded())
	{
		Mesh->SetStaticMesh(tempMesh.Object);
	}
}

// Called when the game starts or when spawned
void ARootAnchor::BeginPlay()
{
	Super::BeginPlay();
}


