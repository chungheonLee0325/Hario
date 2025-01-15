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
	void SetM_HomePosition(const FVector& M_HomePosition);
	void SetM_RootObject(USceneComponent* M_RootComponent);
	void SetM_MovementSpeed(float M_MovementSpeed);

private:
	// 복귀 위치 - Deploy Position or RootPosition
	FVector m_HomePosition;
	
	// 이동시킬 오브젝트 -> 대부분 RootComponent
UPROPERTY()
	USceneComponent* m_RootObject;
	
	// 이동 속도
	float m_MovementSpeed;
};
