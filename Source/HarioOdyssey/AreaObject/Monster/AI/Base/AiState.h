// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "HarioOdyssey/ResourceManager/Hario_Enum.h"

#include "AiState.generated.h"

class UAiFSM;

/**
 * 
 */
UCLASS(Abstract)
class HARIOODYSSEY_API UAiState : public UObject
{
	GENERATED_BODY()

public:
	void SetAiFSM(UAiFSM* AiFSM);
	void SetAiKind(EAiStateType StateType);
	EAiStateType AiStateType() const;

	virtual void Enter() PURE_VIRTUAL(UAiState::Enter,);
	virtual void Execute(float dt) PURE_VIRTUAL(UAiState::Excute,);
	virtual void Exit() PURE_VIRTUAL(UAiState::Exit,);

protected:
	EAiStateType m_AiStateType;

	UPROPERTY()
	UAiFSM* m_AiFSM;	
};
