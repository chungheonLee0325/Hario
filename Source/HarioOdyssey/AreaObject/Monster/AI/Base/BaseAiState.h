﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HarioOdyssey/ResourceManager/HarioGameType.h"
#include "BaseAiState.generated.h"

class ABaseMonster;
class UBaseAiFSM;

/**
 * 
 */
UCLASS(Abstract)
class HARIOODYSSEY_API UBaseAiState : public UObject
{
	GENERATED_BODY()

public:
	void SetAiFSM(UBaseAiFSM* AiFSM);
	void SetAiKind(EAiStateType StateType);
	EAiStateType AiStateType() const;
	virtual void InitState() PURE_VIRTUAL(UBaseAiState::InitState);
	void SetOwner(ABaseMonster* Owner);
	virtual void SetNextState(EAiStateType NextState);
	virtual void Enter() PURE_VIRTUAL(UBaseAiState::Enter,);
	virtual void Execute(float dt) PURE_VIRTUAL(UAiState::Excute,);
	virtual void Exit() PURE_VIRTUAL(UBaseAiState::Exit,);

protected:
	EAiStateType m_AiStateType;

	UPROPERTY()
	UBaseAiFSM* m_AiFSM;	

	UPROPERTY()
	ABaseMonster* m_Owner;

	UPROPERTY()
	EAiStateType m_NextState;
};
