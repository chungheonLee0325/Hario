// Fill out your copyright notice in the Description page of Project Settings.


#include "RegionCoin.h"

#include "Components/SphereComponent.h"
#include "HarioOdyssey/AreaObject/Player/Player_Mario.h"


// Sets default values
ARegionCoin::ARegionCoin()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 코인 가치 초기화
	m_CoinValue = 1;

	// 코인 메시 설정
	if (ItemMesh)
	{
		static ConstructorHelpers::FObjectFinder<UStaticMesh> CoinMeshAsset(TEXT("/Script/Engine.StaticMesh'/Game/_Resource/Object/RegionalCoin/ST_RegionCoin.ST_RegionCoin'"));
		if (CoinMeshAsset.Succeeded())
		{
			ItemMesh->SetStaticMesh(CoinMeshAsset.Object);
		}
		ItemMesh->SetRelativeScale3D(FVector(0.3f));
	}
	if (CollectionSphere)
	{
		CollectionSphere->SetRelativeScale3D(FVector(3.7f));
	}

	//{
	//	static ConstructorHelpers::FObjectFinder<USoundBase> SoundAsset(TEXT("/Script/Engine.SoundWave'/Game/_Resource/Sound/Item/SFX_Mario_Coin.SFX_Mario_Coin'"));
	//	if (SoundAsset.Succeeded())
	//	{
	//		CollectionSound = SoundAsset.Object;
	//	}
	//}
}

// Called when the game starts or when spawned
void ARegionCoin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARegionCoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARegionCoin::ApplyEffect(class APlayer_Mario* Player)
{
	if (Player)
	{
		// 케릭터의 코인 수 증가
		Player->AddRegionCoin(m_CoinValue);
	}
}
