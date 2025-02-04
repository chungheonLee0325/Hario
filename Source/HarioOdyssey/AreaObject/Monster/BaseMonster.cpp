// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMonster.h"

#include "AI/Base/BaseAiFSM.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HarioOdyssey/AreaObject/Player/Player_Mario.h"
#include "HarioOdyssey/AreaObject/Skill/Base/BaseSkill.h"
#include "HarioOdyssey/PathMover/PathMover.h"
#include "HarioOdyssey/PathMover/VerticalMover.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ABaseMonster::ABaseMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	m_PathMover = CreateDefaultSubobject<UPathMover>(TEXT("PathMover"));
	m_VerticalMover = CreateDefaultSubobject<UVerticalMover>(TEXT("VerticalMover"));

	// Death Effect Load
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleAsset(
		TEXT("/Script/Engine.ParticleSystem'/Game/_Resource/FX/Realistic_Starter_VFX_Pack_Vol2/Particles/Destruction/P_Destruction_Electric.P_Destruction_Electric'"));
	if (ParticleAsset.Succeeded())
	{
		DeathEffect = ParticleAsset.Object;
	}
}

// Called when the game starts or when spawned
void ABaseMonster::BeginPlay()
{
	Super::BeginPlay();

	m_SpawnLocation = GetActorLocation();

	// 스킬 인스턴스 생성
	for (auto& Pair : m_StateSkillClasses)
	{
		if (Pair.Value)
		{
			UBaseSkill* NewSkill = NewObject<UBaseSkill>(this, Pair.Value);
			if (NewSkill)
			{
				m_StateSkillInstances.Add(Pair.Key, NewSkill);
				m_SkillInstances.Add(Pair.Value, NewSkill);
			}
		}
	}
	for (auto& skillClass : m_SkillClasses)
	{
		if (skillClass)
		{
			UBaseSkill* NewSkill = NewObject<UBaseSkill>(this, skillClass);
			m_SkillInstances.Add(skillClass, NewSkill);
		}
	}
	if (m_AiFSM != nullptr)
	{
		m_AiFSM->InitStatePool();
	}
}

// Called every frame
void ABaseMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 현재 스킬 업데이트
	if (m_CurrentSkill)
	{
		m_CurrentSkill->OnCastTick(DeltaTime);
	}
}

// Called to bind functionality to input
void ABaseMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseMonster::OnBodyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                      UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                      const FHitResult& SweepResult)
{
	auto player = Cast<APlayer_Mario>(OtherActor);
	if (player != nullptr)
	{
		CalcDamage(1.0f, this, player);
	}
}

UBaseSkill* ABaseMonster::FindSkillByState(EAiStateType StateType) const
{
	if (UBaseSkill* const* SkillPtr = m_StateSkillInstances.Find(StateType))
	{
		return *SkillPtr;
	}
	return nullptr;
}

UBaseSkill* ABaseMonster::FindSkillByClass(TSubclassOf<UBaseSkill> SkillClass)
{
	auto skill = m_SkillInstances.Find(SkillClass);
	if (skill != nullptr)
	{
		return *skill;
	}
	return nullptr;
}

void ABaseMonster::OnDie()
{
	Super::OnDie();

	// FSM 정지
	m_AiFSM->StopFSM();
	// Skill 정지
	if (nullptr != m_CurrentSkill) m_CurrentSkill->CancelCast();
	// 움직임 정지
	StopAll();
	m_VerticalMover->StopVerticalMovement();
	// 몬스터 발사!
	LaunchOnDeath();
	// 콜리전 전환
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TWeakObjectPtr<ABaseMonster> WeakThis = this;

	// Delay 후 폭발
	GetWorld()->GetTimerManager().SetTimer(OnDieHandle, [WeakThis]()
	{
		ABaseMonster* StrongThis = WeakThis.Get();
		// Death Effect
		UGameplayStatics::SpawnEmitterAtLocation(StrongThis->GetWorld(), StrongThis->DeathEffect,
		                                         StrongThis->GetActorLocation());
		if (nullptr != StrongThis)
		{
			StrongThis->Destroy();
		}
	}, DestroyDelayTime, false);
}

void ABaseMonster::LaunchOnDeath()
{
	m_VerticalMover->StartVerticalMovement(GetMesh(), 400, DestroyDelayTime, 2);

	auto point = FMath::RandPointInCircle(1);
	FVector position = GetActorLocation() + FVector(point.X, point.Y, 0.0f) * 600.0f;
	MoveToLocation(position, DestroyDelayTime);
}

// Launch Ver
void ABaseMonster::LaunchOnDeathVer2()
{
	// 캐릭터 물리 설정
	GetMesh()->SetSimulatePhysics(true);

	int LaunchUpwardAngle = FMath::RandRange(LaunchUpwardAngleMin,LaunchUpwardAngleMax);
	
	// 랜덤한 수평 방향 계산
	int RandomAngle = FMath::RandRange(0, 360);
	FVector LaunchDirection(
		FMath::Cos(FMath::DegreesToRadians(RandomAngle)),
		FMath::Sin(FMath::DegreesToRadians(RandomAngle)),
		FMath::Sin(FMath::DegreesToRadians(LaunchUpwardAngle))
	);
	// 발사 속도 계산
	FVector LaunchVelocity = LaunchDirection * LaunchSpeed;
	UE_LOG(LogTemp, Warning, TEXT("Movement Mode: %d"), (int)GetCharacterMovement()->MovementMode);
	UE_LOG(LogTemp, Warning, TEXT("Launch Velocity: %s"), *LaunchVelocity.ToString());

	// Launch와 동시에 회전력 설정
	LaunchCharacter(LaunchVelocity, true, true);
	//GetMesh()->AddTorqueInDegrees(FVector(0, 0, 1000));
}


UBaseAiFSM* ABaseMonster::CreateFSM()
{
	return nullptr;
}

void ABaseMonster::StopRotating()
{
	if (!IsValidForMovement()) return;

	if (m_PathMover)
	{
		m_PathMover->StopRotation();
	}
}

void ABaseMonster::StopMoving()
{
	if (!IsValidForMovement()) return;

	if (m_PathMover)
	{
		m_PathMover->StopMovement();
	}
}

void ABaseMonster::MoveToLocation(const FVector& Target, float Duration, EMovementInterpolationType InterpType)
{
	if (IsValidForMovement())
	{
		m_PathMover->MoveActorTo(Target, Duration, InterpType);
	}
}

void ABaseMonster::MoveToLocationWithSpeed(const FVector& Target, float Speed, EMovementInterpolationType InterpType)
{
	if (IsValidForMovement())
	{
		m_PathMover->MoveActorToWithSpeed(Target, Speed, InterpType);
	}
}

void ABaseMonster::MoveToActor(AActor* Target, float Duration, EMovementInterpolationType InterpType)
{
	if (Target && IsValidForMovement())
	{
		MoveToLocation(Target->GetActorLocation(), Duration, InterpType);
	}
}

void ABaseMonster::MoveToActorWithSpeed(const AActor* Target, float Speed, EMovementInterpolationType InterpType)
{
	if (Target && IsValidForMovement())
	{
		MoveToLocationWithSpeed(Target->GetActorLocation(), Speed, InterpType);
	}
}

void ABaseMonster::MoveComponentToLocation(USceneComponent* ComponentToMove, const FVector& Target,
                                           float Duration, EMovementInterpolationType InterpType)
{
	if (ComponentToMove && IsValidForMovement())
	{
		m_PathMover->MoveComponentTo(ComponentToMove, Target, Duration, InterpType);
	}
}

void ABaseMonster::MoveComponentToLocationWithSpeed(USceneComponent* ComponentToMove, const FVector& Target,
                                                    float Speed, EMovementInterpolationType InterpType)
{
	if (ComponentToMove && IsValidForMovement())
	{
		m_PathMover->MoveComponentToWithSpeed(ComponentToMove, Target, Speed, InterpType);
	}
}

void ABaseMonster::ReturnComponentToOriginal(USceneComponent* ComponentToReturn, float Duration,
                                             EMovementInterpolationType InterpType, bool bStickToGround)
{
	if (ComponentToReturn && IsValidForMovement())
	{
		m_PathMover->ReturnComponentToOriginal(ComponentToReturn, Duration, InterpType, bStickToGround);
	}
}

// ToDo : 매서드 고치면서 기능 통폐합
void ABaseMonster::ReturnComponentToOriginalWithSpeed(USceneComponent* ComponentToReturn, float Speed,
                                                      EMovementInterpolationType InterpType, bool bStickToGround)
{
	if (ComponentToReturn && IsValidForMovement())
	{
		m_PathMover->ReturnComponentToOriginal(ComponentToReturn, Speed, InterpType, bStickToGround);
	}
}

void ABaseMonster::OnMovementFinished()
{
}

void ABaseMonster::OnRotationFinished()
{
}

FVector ABaseMonster::GetM_SpawnLocation() const
{
	return m_SpawnLocation;
}

bool ABaseMonster::IsMoving() const
{
	return m_PathMover && m_PathMover->IsMoving();
}

bool ABaseMonster::IsRotating() const
{
	return m_PathMover && m_PathMover->IsRotating();
}

bool ABaseMonster::IsValidForMovement() const
{
	return IsValid(this) && m_PathMover != nullptr;
}

UBaseSkill* ABaseMonster::GetCurrentSkill()
{
	return m_CurrentSkill;
}

void ABaseMonster::UpdateCurrentSkill(UBaseSkill* NewSkill)
{
	if (nullptr == NewSkill)
	{
		NewSkill = DeQueueSkill();
	}

	if (nullptr == NewSkill)
	{
		return;
	}

	m_CurrentSkill = NewSkill;
}

void ABaseMonster::ClearCurrentSkill()
{
	m_CurrentSkill = nullptr;
}

AActor* ABaseMonster::GetAggroTarget() const
{
	return m_AggroTarget;
}

bool ABaseMonster::CanCastSkill(UBaseSkill* Skill, const AActor* Target) const
{
	return Skill && Skill->CanCast(const_cast<ABaseMonster*>(this), Target);
}

void ABaseMonster::CastSkill(UBaseSkill* Skill, const AActor* Target)
{
	if (CanCastSkill(Skill, Target))
	{
		UpdateCurrentSkill(Skill);
		Skill->OnCastStart(this, Target);
	}
}

void ABaseMonster::EnQueueSkill(UBaseSkill* Skill)
{
	m_SkillQueue.Enqueue(Skill);
}

UBaseSkill* ABaseMonster::DeQueueSkill()
{
	UBaseSkill* Skill = nullptr;
	m_SkillQueue.Dequeue(Skill);

	return Skill;
}

void ABaseMonster::ClearQueueSkill()
{
	m_SkillQueue.Empty();
}

void ABaseMonster::RotateToDirection(const FRotator& Target, float Duration, EMovementInterpolationType InterpType)
{
	if (IsValidForMovement())
	{
		m_PathMover->RotateActorTo(Target, Duration, InterpType);
	}
}

void ABaseMonster::RotateToDirectionWithSpeed(const FRotator& Target, float Speed,
                                              EMovementInterpolationType InterpType)
{
	if (IsValidForMovement())
	{
		m_PathMover->RotateActorToWithSpeed(Target, Speed, InterpType);
	}
}

void ABaseMonster::LookAtLocation(const FVector& Target, float Duration, EMovementInterpolationType InterpType)
{
	if (IsValidForMovement())
	{
		auto safeTarget = GetActorLocation() - (GetActorLocation() - Target).GetSafeNormal2D();
		m_PathMover->LookAtLocationWithActor(safeTarget, Duration, InterpType);
	}
}

void ABaseMonster::LookAtLocationWithSpeed(const FVector& Target, float Speed, EMovementInterpolationType InterpType)
{
	if (IsValidForMovement())
	{
		auto safeTarget = GetActorLocation() - (GetActorLocation() - Target).GetSafeNormal2D();
		m_PathMover->LookAtLocationWithActorSpeed(safeTarget, Speed, InterpType);
	}
}

void ABaseMonster::LookAtComponentToLocation(USceneComponent* ComponentToRotate, const FVector& TargetLocation,
                                             float Duration, EMovementInterpolationType InterpType)
{
	if (IsValidForMovement())
	{
		m_PathMover->LookAtLocationWithComponent(ComponentToRotate, TargetLocation, Duration, InterpType);
	}
}

void ABaseMonster::LookAtComponentToLocationWithSpeed(USceneComponent* ComponentToRotate, const FVector& TargetLocation,
                                                      float Speed, EMovementInterpolationType InterpType)
{
	if (IsValidForMovement())
	{
		m_PathMover->LookAtLocationWithComponent(ComponentToRotate, TargetLocation, Speed, InterpType);
	}
}

void ABaseMonster::StopAll()
{
	if (IsValidForMovement())
	{
		m_PathMover->StopAll();
	}
}
