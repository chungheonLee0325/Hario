// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HarioOdyssey/AreaObject/Skill/Base/BaseSkill.h"
#include "JumpAttack.generated.h"


UCLASS()
class HARIOODYSSEY_API UJumpAttack : public UBaseSkill
{
	GENERATED_BODY()

protected:
	virtual void UpdatePreparePhase(float DeltaTime) override;
	virtual void UpdateCastingPhase(float DeltaTime) override;
	virtual void UpdatePostCastPhase(float DeltaTime) override;

private:
	float Radian = 0.0f;
	float Amplitue = 0.3f;
	FRotator Rotation = FRotator(0.0f, 0.f,60.f);
	float Period = 8.f;
};
