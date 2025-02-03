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
    PrimaryActorTick.bCanEverTick = true;

    BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
    SetRootComponent(BoxComp);

    HatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Hat"));
    HatMesh->SetupAttachment(BoxComp);

    InitialDirection = FVector::ZeroVector;
    
    bSpinning = false;
    bExpanding = false;
    OrbitRadius = 50.0f;
    OrbitSpeed = 360.0f / 1.0f;
    MaxOrbitRadius = 300.0f;
    SpiralSpeed = 100.0f;
    CurrentAngle = 0.0f;
    ExpansionTime = 2.0f;
    CurrentTime = 0.0f;
    bReturning = false;
    bIsThrownStraight = false;
}

// Called when the game starts or when spawned
void AHatProjectile::BeginPlay()
{
    Super::BeginPlay();
    StartLocation = GetActorLocation();
    UE_LOG(LogTemp, Warning, TEXT("모자 생성 완료, 초기 위치: %s"), *StartLocation.ToString());
}

// Called every frame
void AHatProjectile::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (bExpanding) // 🔹 R 키: 나선형 궤적
    {
        CurrentAngle += OrbitSpeed * DeltaTime;
        if (CurrentAngle >= 360.0f) CurrentAngle -= 360.0f;

        float GrowthFactor = FMath::Clamp(CurrentTime / ExpansionTime, 0.0f, 1.0f);
        OrbitRadius = FMath::Lerp(50.0f, MaxOrbitRadius, GrowthFactor);

        FVector Offset = FVector(
            FMath::Cos(FMath::DegreesToRadians(CurrentAngle)) * OrbitRadius,
            FMath::Sin(FMath::DegreesToRadians(CurrentAngle)) * OrbitRadius,
            0.0f
        );

        if (OwnerActor)
        {
            FVector CenterLocation = OwnerActor->GetActorLocation();
            SetActorLocation(CenterLocation + Offset);
        }

        FRotator Rotation = GetActorRotation();
        Rotation.Yaw += OrbitSpeed * DeltaTime * 3;
        SetActorRotation(Rotation);

        CurrentTime += DeltaTime;
        if (CurrentTime >= ExpansionTime)
        {
            bExpanding = false;
            bReturning = true;
            UE_LOG(LogTemp, Warning, TEXT("모자 나선 이동 종료, 돌아오기 시작"));
        }
    }
    else if (bIsThrownStraight) // 🔹 E 키: 직선 이동
    {
        FVector NewLocation = GetActorLocation() + (InitialDirection * Speed * DeltaTime);
        SetActorLocation(NewLocation);

        FRotator Rotation = GetActorRotation();
        Rotation.Yaw += OrbitSpeed * DeltaTime * 4;
        SetActorRotation(Rotation);

        CurrentTime += DeltaTime;
        UE_LOG(LogTemp, Warning, TEXT("모자 직선 이동 중... 현재 위치: %s"), *NewLocation.ToString());

        if (CurrentTime >= FlightTime || FVector::Dist(StartLocation, GetActorLocation()) >= 500.0f)
        {
            UE_LOG(LogTemp, Warning, TEXT("모자 직선 이동 종료, 돌아오기 시작"));
            bReturning = true;  
            bIsThrownStraight = false;  // ✅ 직선 이동을 멈추고 되돌아오는 상태로 전환
        }
    }
    else if (bReturning) // 🔹 되돌아오는 상태
    {
        if (OwnerActor)
        {
            FVector DirectionToOwner = (OwnerActor->GetActorLocation() - GetActorLocation()).GetSafeNormal();
            FVector NewLocation = GetActorLocation() + (DirectionToOwner * Speed * 2.0f * DeltaTime); // ✅ 되돌아올 때 속도 증가
            SetActorLocation(NewLocation);

            UE_LOG(LogTemp, Warning, TEXT("모자가 되돌아오는 중... 방향: %s, 현재 위치: %s"), *DirectionToOwner.ToString(), *NewLocation.ToString());

            if (FVector::Dist(OwnerActor->GetActorLocation(), GetActorLocation()) <= 50.0f)
            {
                UE_LOG(LogTemp, Warning, TEXT("모자가 플레이어에게 돌아옴"));
                if (APlayer_Mario* Player = Cast<APlayer_Mario>(OwnerActor))
                {
                    Player->OnHatReturned(this);
                }
                Destroy();
            }
        }
    }
}

// 모자 던질 때 초기화
void AHatProjectile::InitializeHat(FVector Direction, APlayer_Mario* NewOwner, bool bIsSpinThrow)
{
    InitialDirection = Direction;
    OwnerActor = NewOwner;
    HatMesh->SetSimulatePhysics(true);

    FVector Velocity = Direction * 1000.f;
    HatMesh->AddImpulse(Velocity, NAME_None, true);

    CurrentTime = 0.0f;
    bReturning = false;
    StartLocation = GetActorLocation();

    if (bIsSpinThrow) // 🔹 R 키일 때 나선형 적용
    {
        bExpanding = true;
        bIsThrownStraight = false;
    }
    else // 🔹 E 키일 때 직선 이동
    {
        bExpanding = false;
        bIsThrownStraight = true;
    }

    UE_LOG(LogTemp, Warning, TEXT("모자 던짐 - bIsSpinThrow: %s"), bIsSpinThrow ? TEXT("true") : TEXT("false"));
}

