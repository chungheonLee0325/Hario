#include "HatProjectile.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "HarioOdyssey/AreaObject/Monster/Monster.h"
#include "HarioOdyssey/AreaObject/Player/Player_Mario.h"

// Sets default values
AHatProjectile::AHatProjectile()
{
	// Set this actor to call Tick() every frame
	PrimaryActorTick.bCanEverTick = true;

	HatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HatMesh"));
	RootComponent = HatMesh;

	// 기본 값 설정
	ThrowVelocity = FVector(1000.f, 0.f, 0.f);

	
	// 충돌체와 프로젝타일 이동 컴포넌트 설정
	CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	CollisionComponent->InitSphereRadius(15.0f);
	CollisionComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));
	RootComponent = CollisionComponent;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// 플레이어 참조 초기화
	Player = nullptr;
}

// Called when the game starts or when spawned
void AHatProjectile::BeginPlay()
{
	Super::BeginPlay();
  
	// 던져지기 전에 몬스터를 찾기 (예: 게임 시작 시, 타겟 몬스터 지정)
	TArray<AActor*> FoundMonsters;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMonster::StaticClass(), FoundMonsters);
  
	if (FoundMonsters.Num() > 0)
	{
		TargetMonster = FoundMonsters[0];  // 첫 번째 몬스터 선택
	}
  
	// 던져지기
	HatMesh->SetPhysicsLinearVelocity(ThrowVelocity);

	// 충돌 이벤트 바인딩
	//?오류
	//CollisionComponent->OnComponentHit.AddDynamic(this, &AHatProjectile::OnHit);

	// 모자를 던진 플레이어를 찾기
	AActor* owner = GetOwner();
	if (owner)
	{
		Player = Cast<APlayer_Mario>(owner);
	}
}

// Called every frame
void AHatProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 몬스터와의 거리 체크 및 붙이기
	CheckAndAttachToMonster();
}


	

void AHatProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, 
						   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
						   bool bFromSweep, const FHitResult& SweepResult)
{
	// 몬스터와 충돌했을 경우
	if (OtherActor && OtherActor->IsA(AMonster::StaticClass()))
	{
		AMonster* HitMonster = Cast<AMonster>(OtherActor);
        
		if (HitMonster)
		{
			AttachToMonster(HitMonster);
			// 모자 던진 플레이어를 일시적으로 숨기기
			HidePlayerTemporarily();
			// 모자 제거
			Destroy();
		}
	}
}

void AHatProjectile::AttachToMonster(AMonster* Monster)
{
	
}

// 몬스터와의 거리를 체크하고 붙이기
void AHatProjectile::CheckAndAttachToMonster()
{
	if (TargetMonster && IsNearMonster())
	{
		FVector TargetLocation = TargetMonster->GetActorLocation();
		FVector AttachLocation = TargetLocation + AttachOffset;  // 몬스터 위쪽에 위치 설정
        
		// 모자를 몬스터 위쪽에 고정
		SetActorLocationAndRotation(AttachLocation, TargetMonster->GetActorRotation());
	
		// 필요 시 물리 엔진을 끄거나 추가 로직
		HatMesh->SetSimulatePhysics(false);
	}
}

// 몬스터와의 거리가 AttachDistance 이내인지 확인하는 함수
bool AHatProjectile::IsNearMonster()
{
	if (TargetMonster)
	{
		float Distance = FVector::Dist(GetActorLocation(), TargetMonster->GetActorLocation());
		return Distance <= AttachDistance;
	}
	return false;
}

void AHatProjectile::HidePlayerTemporarily()
{
	if (Player)
	{
		// 플레이어를 숨기기
		Player->SetActorHiddenInGame(true);

		// 일정 시간 후 다시 보이게 하기
		GetWorld()->GetTimerManager().SetTimer(UnhideTimerHandle, this, &AHatProjectile::ShowPlayerAgain, HiddenDuration, false);
	}
}

void AHatProjectile::ShowPlayerAgain()
{
	if (Player)
	{
		// 플레이어를 다시 보이게 하기
		Player->SetActorHiddenInGame(false);
	}
}