// Fill out your copyright notice in the Description page of Project Settings.


#include "Heart.h"

#include "HarioOdyssey/AreaObject/Player/Player_Mario.h"


// Sets default values
AHeart::AHeart()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
 
	// Heart 가치 초기화
	m_HeartValue = 1.0f;

	// Heart 메시 설정
	if (ItemMesh)
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> HeartMeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/_Resource/Object/LifeUpItem/ST_LifeUpItem.ST_LifeUpItem'"));
		if (HeartMeshAsset.Succeeded())
		{
			ItemMesh->SetStaticMesh(HeartMeshAsset.Object);
		}
		ItemMesh->SetRelativeScale3D(FVector(0.3f));
	}
	
}

// Called when the game starts or when spawned
void AHeart::BeginPlay()
{
	Super::BeginPlay();
	
}

void AHeart::ApplyEffect(APlayer_Mario* Player)
{
	Player->IncreaseHP(m_HeartValue);
}

// Called every frame
void AHeart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

