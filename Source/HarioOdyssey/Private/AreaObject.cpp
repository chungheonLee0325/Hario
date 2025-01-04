// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaObject.h"

// Sets default values
AAreaObject::AAreaObject()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AAreaObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAreaObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAreaObject::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

