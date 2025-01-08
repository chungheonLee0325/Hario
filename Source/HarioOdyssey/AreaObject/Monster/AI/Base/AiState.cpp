// Fill out your copyright notice in the Description page of Project Settings.

#include "AiState.h"

#include "AiFSM.h"


void UAiState::SetAiFSM(UAiFSM* AiFSM)
{
	m_AiFSM = AiFSM;
}

void UAiState::SetAiKind(EAiStateType StateType)
{
	m_AiStateType = StateType;
}

EAiStateType UAiState::AiStateType() const
{
	return m_AiStateType;
}
