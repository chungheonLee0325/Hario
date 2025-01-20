// Fill out your copyright notice in the Description page of Project Settings.


#include "JumpAttack.h"

#include "HarioOdyssey/AreaObject/Monster/BaseMonster.h"


void UJumpAttack::UpdatePreparePhase(float DeltaTime)
{
	Super::UpdatePreparePhase(DeltaTime);

	Radian += DeltaTime * Period;
	Radian = FMath::Fmod(Radian, 2*PI);
	auto tempR = Rotation * Amplitue * FMath::Sin(Radian);
	m_Caster->SetActorRotation(tempR);
}

void UJumpAttack::UpdateCastingPhase(float DeltaTime)
{
	Super::UpdateCastingPhase(DeltaTime);
}

void UJumpAttack::UpdatePostCastPhase(float DeltaTime)
{
	Super::UpdatePostCastPhase(DeltaTime);
}
