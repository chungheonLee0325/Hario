﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HarioOdyssey/AreaObject/Monster/AI/Base/AiState.h"
#include "AggroWait.generated.h"

class AAreaObject;
/**
 * 
 */
UCLASS()
class HARIOODYSSEY_API UAggroWait : public UAiState
{
	GENERATED_BODY()

public:
	void SetM_DetectRange(float Range);
	void SetM_WaitTime(float Time);

	virtual void InitState() override;
	virtual void Enter() override;
	virtual void Execute(float dt) override;
	virtual void Exit() override;

private:
	bool DetectPlayer() const;
	float m_WaitTime = 0.f;
	float m_WaitTimeElapsed;
	float m_DetectCycleTime = 1.f;
	float m_DetectCycleTimeElapsed = 0.f;
	float m_DetectRange = 0.f;
	EAiStateType m_NextAiState;

	UPROPERTY()
	AAreaObject *m_Target = nullptr;
};