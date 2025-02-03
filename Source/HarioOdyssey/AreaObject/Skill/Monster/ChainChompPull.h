// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HarioOdyssey/AreaObject/Skill/Base/BaseSkill.h"
#include "ChainChompPull.generated.h"

class AHarioGameMode;
class AChainChomp;
/**
 * 
 */
UCLASS()
class HARIOODYSSEY_API UChainChompPull : public UBaseSkill
{
	GENERATED_BODY()

public:
	UChainChompPull();

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
	FVector m_OriginalPosition;
	UPROPERTY()
	FVector m_PullBackPosition;

	UPROPERTY(EditAnywhere, Category = "Skill Setting")
	float PullBackDistance;

	UPROPERTY(EditAnywhere, Category = "Skill Setting")
	float RotateTime;

	UPROPERTY(EditAnywhere, Category = "Skill Setting")
	float PullTime;
};
