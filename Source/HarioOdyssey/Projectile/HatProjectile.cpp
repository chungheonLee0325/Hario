#include "HatProjectile.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "HarioOdyssey/AreaObject/Player/Player_Mario.h"

// Sets default values
AHatProjectile::AHatProjectile()
{
	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);

	//모자 메시 컴포넌트 초기화
	HatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hat"));
	HatMesh->SetupAttachment(BoxComp);

	//초기 방향 설정
	InitialDirection = FVector::ZeroVector;

	//회전
	OrbitRadius = 200.0f;    // 회전 반지름 초기값
	OrbitSpeed = 360.0f / 2.0f;     // 1초에 180도 회전
	CurrentAngle = 0.0f;     // 시작 각도
	bSpinning = false;		// 기본적으로 회전 상태가 아님
}

// Called when the game starts or when spawned
void AHatProjectile::BeginPlay()
{
	Super::BeginPlay();

	//시작 위치 
	StartLocation = GetActorLocation();
}

void AHatProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bSpinning) // 회전 상태
	{
		UE_LOG(LogTemp, Warning, TEXT("모자 스핀 성공"),CurrentAngle);
		CurrentAngle += OrbitSpeed * DeltaTime; // 각도 업데이트
		if (CurrentAngle >= 360.0f)
		{
			CurrentAngle -= 360.0f;
		}

		// 플레이어 주변 회전
		FVector Offset = FVector(
			FMath::Cos(FMath::DegreesToRadians(CurrentAngle)) * OrbitRadius,
			FMath::Sin(FMath::DegreesToRadians(CurrentAngle)) * OrbitRadius,
			0.0f
		);

		if (!OwnerActor)
		{
			UE_LOG(LogTemp, Warning, TEXT("HatInstance가 이미 존재함"));
			return;
		}

		// 회전 지속 시간 검사
		CurrentTime += DeltaTime;
		if (CurrentTime >= 2.0f) // 2초가 지나면 회전 종료
		{
			bSpinning = false;
			UE_LOG(LogTemp, Warning, TEXT("모자 회전 종료"));
		}
		
		FVector CenterLocation = OwnerActor->GetActorLocation(); // 소유자 위치 기준으로 회전
		SetActorLocation(CenterLocation + Offset);

		if (FVector::Dist(OwnerActor->GetActorLocation(), GetActorLocation()) <= 10.0f)
		{
			if (APlayer_Mario* Player = Cast<APlayer_Mario>(OwnerActor))
			{
				Player->OnHatReturned(this);
			}
			Destroy();
		}
	}
	else if (!bReturning) // 던져지는 상태
	{
		CurrentTime += DeltaTime;

		// 전방 이동
		FVector NewLocation = GetActorLocation() + (InitialDirection * Speed * DeltaTime);
		SetActorLocation(NewLocation);

		if (CurrentTime >= FlightTime)
		{
			bReturning = true;
		}
	}
	else if (bReturning) // 돌아오는 상태
	{
		if (OwnerActor)
		{
			FVector DirectionToOwner = (OwnerActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			FVector NewLocation = GetActorLocation() + (DirectionToOwner * Speed * DeltaTime);
			SetActorLocation(NewLocation);

			if (FVector::Dist(OwnerActor->GetActorLocation(), GetActorLocation()) <= 10.0f)
			{
				if (APlayer_Mario* Player = Cast<APlayer_Mario>(OwnerActor))
				{
					Player->OnHatReturned(this);
				}
				Destroy();
			}
		}
	}
	// if (!bReturning)
	// {
	// 	// 시간 업데이트
	// 	CurrentTime += DeltaTime;
	//
	// 	// 모자가 전방으로 이동
	// 	FVector NewLocation = GetActorLocation() + (InitialDirection * Speed * DeltaTime);
	// 	SetActorLocation(NewLocation);
	//
	// 	// 날아가는 시간이 끝나면 돌아오기 시작
	// 	if (CurrentTime >= FlightTime)
	// 	{
	// 		bReturning = true;
	// 	}
	// }
	// else
	// {
	// 	if (OwnerActor)
	// 	{
	// 		// 모자가 돌아오기
	// 		FVector DirectionToOwner = (OwnerActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	// 		FVector NewLocation = GetActorLocation() + (DirectionToOwner * Speed * DeltaTime);
	// 		SetActorLocation(NewLocation);
	//
	//
	// 		// 원래 위치로 돌아오면 삭제
	// 		if (FVector::Dist(OwnerActor->GetActorLocation(), GetActorLocation()) <= 10.0f)
	// 		{
	// 			// 소유자에게 돌아오면 파괴
	// 			 if (APlayer_Mario* Player = Cast<APlayer_Mario>(OwnerActor))
	// 			{
	// 				Player->OnHatReturned(this);
	// 			 }
	// 			
	// 			Destroy();
	// 		
	// 		}
	// 	}
	// }
}


void AHatProjectile::InitializeHat(FVector Direction, APlayer_Mario* NewOwner)
{
	//모자 초기 방향. 주인 설정
	InitialDirection = Direction;
	OwnerActor = NewOwner; // 클래스 멤버 변수에 할당
	;
	// 모자 발사 전에 물리 시뮬레이션을 활성화
	HatMesh->SetSimulatePhysics(true);

	//모자 발사
	FVector Velocity = Direction * 1000.f;
	HatMesh->AddImpulse(Velocity, NAME_None, true);

	//초기화 상태
	CurrentTime = 0.0f;
	bReturning = false;
	StartLocation = GetActorLocation();

	// StartLocation = GetActorLocation();
	// OwnerActor = GetOwner();
}


