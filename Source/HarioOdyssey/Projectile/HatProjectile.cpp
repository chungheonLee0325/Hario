#include "HatProjectile.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

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


	if (!bReturning)
	{
		// 시간 업데이트
		CurrentTime += DeltaTime;

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
		if (OwnerActor)
		{
			// 모자가 돌아오기
			FVector DirectionToOwner = (OwnerActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			FVector NewLocation = GetActorLocation() + (DirectionToOwner * Speed * DeltaTime);
			SetActorLocation(NewLocation);


			// 원래 위치로 돌아오면 삭제
			if (FVector::Dist(OwnerActor->GetActorLocation(), GetActorLocation()) <= 10.0f)
			{
				Destroy();
			}
		}
	}
}


void AHatProjectile::InitializeHat(FVector Direction, AActor* NewOwner)
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
