// Fill out your copyright notice in the Description page of Project Settings.


#include "HatProjectile.h"

#include "HarioOdyssey/AreaObject/Player/Player_Mario.h"


// Sets default values
AHatProjectile::AHatProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AHatProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}
void AHatProjectile::InitHatProjectile(APlayer_Mario* OwnerCharacter_)
{
	OwnerCharacter = OwnerCharacter_;
}

// Called every frame
void AHatProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsReturning && OwnerCharacter)
	{
		// 캐릭터로 돌아오는 로직
		FVector DirectionToOwner = (OwnerCharacter->GetActorLocation() - GetActorLocation()).GetSafeNormal();
		FVector ReturnVelocity = DirectionToOwner * ReturnSpeed * DeltaTime;

		UPrimitiveComponent* HatRoot = Cast<UPrimitiveComponent>(GetRootComponent());
		if (HatRoot)
		{
			HatRoot->AddForce(ReturnVelocity);
		}

		// 캐릭터와의 거리 확인
		if (FVector::Dist(GetActorLocation(), OwnerCharacter->GetActorLocation()) < 50.0f)
		{
			//OwnerCharacter->bIsHatThrown = false;
			Destroy();
		}
	}
}

void AHatProjectile::SetReturning(bool bReturning)
{
	bIsReturning = bReturning;
}