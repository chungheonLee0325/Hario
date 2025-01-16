#include "HatProjectile.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"

// Sets default values
AHatProjectile::AHatProjectile()
{
	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	Speed = 600.0f;        // 모자의 속도
	FlightTime = 2.0f;     // 모자가 날아가는 총 시간
	CurrentTime = 0.0f;
	bReturning = false;
	
}

// Called when the game starts or when spawned
void AHatProjectile::BeginPlay()
{
	Super::BeginPlay();

	StartLocation = GetActorLocation();
}

// Called every frame
void AHatProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 시간 업데이트
	CurrentTime += DeltaTime;

	if (!bReturning)
	{
		// 모자가 전방으로 이동
		FVector NewLocation = GetActorLocation() + (InitialDirection * Speed * DeltaTime);
		SetActorLocation(NewLocation);

		// 날아가는 시간이 끝나면 돌아오기 시작
		if (CurrentTime >= FlightTime)
		{
			bReturning = true;
		}
	}
	else
	{
		// 모자가 돌아오기
		FVector DirectionToOwner = (StartLocation - GetActorLocation()).GetSafeNormal();
		FVector NewLocation = GetActorLocation() + (DirectionToOwner * Speed * DeltaTime);
		SetActorLocation(NewLocation);

		// 원래 위치로 돌아오면 삭제
		if (FVector::Dist(GetActorLocation(), StartLocation) <= 10.0f)
		{
			Destroy();
		}
	}
	
}



void AHatProjectile::InitializeHat(FVector ForwardDirection)
{
	InitialDirection = ForwardDirection;
	CurrentTime = 0.0f;
	bReturning = false;
}

	
