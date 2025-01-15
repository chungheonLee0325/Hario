// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseMonster.h"

#include "HarioOdyssey/AreaObject/Skill/Base/BaseSkill.h"
#include "HarioOdyssey/PathMover/PathMover.h"


// Sets default values
ABaseMonster::ABaseMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_PathMover = CreateDefaultSubobject<UPathMover>(TEXT("PathMover"));
}

// Called when the game starts or when spawned
void ABaseMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABaseMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UBaseSkill* ABaseMonster::GetSkillByAiState(EAiStateType StateType)
{
	if (UBaseSkill** data = m_SkillByState.Find(StateType))
	{
		return *data;
	}
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

void ABaseMonster::LookAtLocation(const FVector& Target, float Speed)
{
	if (!IsValidForMovement()) return;
    
	if (m_PathMover)
	{
		//m_PathMover->RotateActorToward(Target, Speed);
	}
}

void ABaseMonster::LookAtActor(AActor* Target, float Speed)
{
	if (!Target || !IsValidForMovement()) return;
    
	LookAtLocation(Target->GetActorLocation(), Speed);
}

void ABaseMonster::ComponentLookAtLocation(USceneComponent* Component, const FVector& Target, float Speed)
{
	if (!Component || !IsValidForMovement()) return;

	if (m_PathMover)
	{
		//m_PathMover->RotateComponentToward(Component, Target, Speed);
	}
}

void ABaseMonster::ComponentLookAtActor(USceneComponent* Component, AActor* Target, float Speed)
{
	if (!Target || !Component || !IsValidForMovement()) return;

	ComponentLookAtLocation(Component, Target->GetActorLocation(), Speed);
}

void ABaseMonster::MoveToLocation(const FVector& Target, float Duration, EMovementInterpolationType InterpType)
{
	if (!IsValidForMovement()) return;
    
	if (m_PathMover)
	{
		m_PathMover->MoveActorTo(Target, Duration, InterpType);
	}
}

void ABaseMonster::MoveToActor(AActor* Target, float Duration, EMovementInterpolationType InterpType)
{
	if (!Target || !IsValidForMovement()) return;
    
	MoveToLocation(Target->GetActorLocation(), Duration, InterpType);
}

void ABaseMonster::MoveComponentToLocation(USceneComponent* ComponentToMove, const FVector& Target, 
										 float Duration, EMovementInterpolationType InterpType)
{
	if (!ComponentToMove || !IsValidForMovement()) return;
    
	if (m_PathMover)
	{
		m_PathMover->MoveComponentTo(ComponentToMove, Target, Duration, InterpType);
	}
}

void ABaseMonster::MoveComponentToActor(USceneComponent* ComponentToMove, AActor* Target, float Duration,
	EMovementInterpolationType InterpType)
{
	if (!Target || !IsValidForMovement()) return;
    
	MoveComponentToLocation(ComponentToMove,Target->GetActorLocation(), Duration, InterpType);
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
