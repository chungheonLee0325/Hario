﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "ChainChomp.h"

#include "Components/SphereComponent.h"


// Sets default values
AChainChomp::AChainChomp()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempRoot(TEXT("/Script/Engine.SkeletalMesh'/Game/_Resource/Monster/ChainChomp/Peg.Peg'"));
	if (tempRoot.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempRoot.Object);
	}
	GetMesh()->SetRelativeScale3D(FVector(0.3f));
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f,0.0f,-60.0f), FRotator(0.0f,0.0f,-90.0f));
	
	ChainChompRoot = CreateDefaultSubobject<USceneComponent>("ChainChompRoot");
	ChainChompRoot->SetRelativeLocation(FVector(0.0f,0.0f,80.0f));
	ChainChompRoot->SetupAttachment(RootComponent);
	
	ChainChompMesh = CreateDefaultSubobject<USkeletalMeshComponent>("ChainChomp");
	ChainChompMesh->SetupAttachment(ChainChompRoot);
	ChainChompMesh->SetRelativeScale3D(FVector(0.3f));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempCCBody(TEXT("/Script/Engine.SkeletalMesh'/Game/_Resource/Monster/ChainChomp/ChainChomp2/ChainChomp2.ChainChomp2'"));
	if (tempCCBody.Succeeded())
	{
		ChainChompMesh->SetSkeletalMesh(tempCCBody.Object);
	}
	
	ChainChompSphere = CreateDefaultSubobject<USphereComponent>("ChainChompSphere");
	ChainChompSphere->SetupAttachment(ChainChompRoot);
	
}

// Called when the game starts or when spawned
void AChainChomp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChainChomp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AChainChomp::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
