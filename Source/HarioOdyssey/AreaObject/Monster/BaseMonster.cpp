// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMonster.h"

#include "AI/Base/BaseAiFSM.h"
#include "HarioOdyssey/AreaObject/Player/Player_Mario.h"
#include "HarioOdyssey/AreaObject/Skill/Base/BaseSkill.h"
#include "HarioOdyssey/PathMover/PathMover.h"


// Sets default values
ABaseMonster::ABaseMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	m_PathMover = CreateDefaultSubobject<UPathMover>(TEXT("PathMover"));
}

// Called when the game starts or when spawned
void ABaseMonster::BeginPlay()
{
	Super::BeginPlay();
	
	m_SpawnLocation = GetActorLocation();

	// 스킬 인스턴스 생성
	for (auto& Pair : StateSkillMap)
	{
		if (Pair.Value)
		{
			UBaseSkill* NewSkill = NewObject<UBaseSkill>(this, Pair.Value);
			if (NewSkill)
			{
				m_StateSkillInstances.Add(Pair.Key, NewSkill);
			}
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
	if (UBaseSkill* CurrentSkill = GetSkillByState(EAiStateType::Attack))
	{
		CurrentSkill->OnCastTick(DeltaTime);
	}
}

// Called to bind functionality to input
void ABaseMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABaseMonster::OnBodyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	auto player = Cast<APlayer_Mario>(OtherActor);
	if (player != nullptr)
	{
		CalcDamage(1.0f,this,player);
	}
}

UBaseSkill* ABaseMonster::GetSkillByState(EAiStateType StateType) const
{
	if (UBaseSkill* const* SkillPtr = m_StateSkillInstances.Find(StateType))
	{
		return *SkillPtr;
	}
	return nullptr;
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
	// TODO: 현재 State에 맞는 스킬 반환
	return nullptr;
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
		Skill->OnCastStart(this, Target);
	}
}

void ABaseMonster::RotateToDirection(const FRotator& Target, float Duration, EMovementInterpolationType InterpType)
{
	if (IsValidForMovement())
	{
		m_PathMover->RotateActorTo(Target, Duration, InterpType);
	}
}

void ABaseMonster::RotateToDirectionWithSpeed(const FRotator& Target, float Speed, EMovementInterpolationType InterpType)
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
		m_PathMover->LookAtLocationWithActor(Target, Duration, InterpType);
	}
}

void ABaseMonster::LookAtLocationWithSpeed(const FVector& Target, float Speed, EMovementInterpolationType InterpType)
{
	if (IsValidForMovement())
	{
		m_PathMover->LookAtLocationWithActorSpeed(Target, Speed, InterpType);
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
