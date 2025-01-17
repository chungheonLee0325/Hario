﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAiState.h"

#include "HarioOdyssey/AreaObject/Monster/BaseMonster.h"
#include "BaseAiFSM.h"


void UBaseAiState::SetAiFSM(UBaseAiFSM* AiFSM)
{
	m_AiFSM = AiFSM;
}

void UBaseAiState::SetAiKind(EAiStateType StateType)
{
	m_AiStateType = StateType;
}

EAiStateType UBaseAiState::AiStateType() const
{
	return m_AiStateType;
}

void UBaseAiState::SetOwner(ABaseMonster* Owner)
{
	m_Owner = Owner;
}

void UBaseAiState::SetNextState(EAiStateType NextState)
{
	m_NextState = NextState;
}
