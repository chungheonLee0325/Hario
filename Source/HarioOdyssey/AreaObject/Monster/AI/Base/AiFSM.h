// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HarioOdyssey/ResourceManager/HarioGameType.h"
#include "AiFSM.generated.h"

class UAiState;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HARIOODYSSEY_API UAiFSM : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UAiFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void AddState(EAiStateType StateType,UAiState* State);
	
	void ChangeState(EAiStateType StateType);

	bool IsExistState(EAiStateType StateType) const;

	virtual void UpdateState(float dt);
	

	
	
protected:
	
UPROPERTY()
	TMap<EAiStateType, UAiState*> m_AiStates;
	
UPROPERTY()
	UAiState* m_CurrentState = nullptr;
UPROPERTY()
	UAiState* m_PreviousState = nullptr;
};
