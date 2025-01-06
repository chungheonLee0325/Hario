// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaObject.h"

#include "HarioOdyssey/Contents/HarioGameInstance.h"

// Sets default values
AAreaObject::AAreaObject()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Health Component 초기화
	m_Health = CreateDefaultSubobject<UHealth>(TEXT("Health"));
}


// Called when the game starts or when spawned
void AAreaObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void AAreaObject::PostInitProperties()
{
	Super::PostInitProperties();

	dt_AreaObject = Cast<UHarioGameInstance>(GetGameInstance())->GetDataAreaObject(m_AreaObjectID);

	float HPMax = 1.0f;
	if (dt_AreaObject != nullptr)
	{
		HPMax = dt_AreaObject->HPMax;
	}
	
	m_Health->InitHealth(HPMax);
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

