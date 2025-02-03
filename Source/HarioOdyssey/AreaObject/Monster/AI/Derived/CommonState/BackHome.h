// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HarioOdyssey/AreaObject/Monster/AI/Base/BaseAiState.h"
#include "BackHome.generated.h"

/**
 * 
 */
UCLASS()
class HARIOODYSSEY_API UBackHome : public UBaseAiState
{
	GENERATED_BODY()

public:
	virtual void InitState() override;
	virtual void Enter() override;
	virtual void Execute(float dt) override;
	virtual void Exit() override;
	void PerformJumpSequence();

private:
	// 이동 속도
	float m_MovementSpeed;
	float BackHomeDuration;
	float BackHomeElapsedTime;
	FTimerHandle FirstJumpHandle;
	FTimerHandle SecondJumpHandle;
	float JumpHeight = 150.f;
	float FirstJumpDelay;
	float SecondJumpDelay;
	float RotateTime;
};
