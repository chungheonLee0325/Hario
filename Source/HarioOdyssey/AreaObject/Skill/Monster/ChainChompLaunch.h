﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HarioOdyssey/AreaObject/Skill/Base/BaseSkill.h"
#include "ChainChompLaunch.generated.h"

class AHarioGameMode;
class AChainChomp;
/**
 * 
 */
UCLASS()
class HARIOODYSSEY_API UChainChompLaunch : public UBaseSkill
{
	GENERATED_BODY()

public:
	UChainChompLaunch();

	UFUNCTION(BlueprintCallable, Category = "Skill")
	void BlockByObject();
	UFUNCTION(BlueprintCallable, Category = "Skill")
	void ClearTimer1();

protected:
	virtual void OnPhaseChanged(ESkillPhase NewPhase) override;
	virtual void UpdatePreparePhase(float DeltaTime) override;
	virtual void UpdateCastingPhase(float DeltaTime) override;
	virtual void UpdatePostCastPhase(float DeltaTime) override;

private:
	UPROPERTY()
	AChainChomp* m_ChainChomp;
	UPROPERTY()
	AHarioGameMode* m_GameMode;
	UPROPERTY()
	FTimerHandle Handle1;

	UPROPERTY(EditAnywhere, Category = "Skill Setting")
	float RotateTime;

	UPROPERTY(EditAnywhere, Category = "Skill Setting")
	float LaunchTime;
};
