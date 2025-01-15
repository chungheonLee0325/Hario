﻿// Fill out your copyright notice in the Description page of Project Settings.


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

