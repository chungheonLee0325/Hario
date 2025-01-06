// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Mario.h"


// Sets default values
APlayer_Mario::APlayer_Mario()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayer_Mario::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayer_Mario::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayer_Mario::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

