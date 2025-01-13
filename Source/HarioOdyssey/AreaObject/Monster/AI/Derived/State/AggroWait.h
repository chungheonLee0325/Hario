// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HarioOdyssey/AreaObject/Monster/AI/Base/AiState.h"
#include "AggroWait.generated.h"

/**
 * 
 */
UCLASS()
class HARIOODYSSEY_API UAggroWait : public UAiState
{
	GENERATED_BODY()

public:
	void SetDetectRange(float Range);
	void SetWaitTime(float Time);

	virtual void InitState() override;
	virtual void Enter() override;
	virtual void Execute(float dt) override;
	virtual void Exit() override;

private:
	float m_WaitTime = 0.f;
	float m_DetectRange = 0.f;
};