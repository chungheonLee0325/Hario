// Fill out your copyright notice in the Description page of Project Settings.


#include "Monster.h"

#include "HarioOdyssey/AreaObject/Skill/Base/BaseSkill.h"


// Sets default values
AMonster::AMonster()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMonster::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMonster::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMonster::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UBaseSkill* AMonster::GetSkillByAiState(EAiStateType StateType)
{
	if (UBaseSkill** data = m_SkillByState.Find(StateType))
	{
		return *data;
	}
	return nullptr;
}

