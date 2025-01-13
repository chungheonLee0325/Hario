// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HarioOdyssey/AreaObject/Monster/AI/Base/AiState.h"
#include "CommonAttack.generated.h"

/**
 * 
 */
UCLASS()
class HARIOODYSSEY_API UCommonAttack : public UAiState
{
	GENERATED_BODY()

public:
	virtual void InitState() override;
	virtual void Enter() override;
	virtual void Execute(float dt) override;
	virtual void Exit() override;


private:
	
	
};
