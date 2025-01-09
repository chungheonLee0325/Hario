// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HarioOdyssey/ResourceManager/HarioGameType.h"
#include "AiFSM.generated.h"

class AMonster;
class UAiState;

UCLASS(Abstract,ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
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

	virtual void InitStatePool() PURE_VIRTUAL(UAiFSM::InitStatePool,);

public:
	template<typename T>
	static T* CreateState(UObject* Outer, AMonster* Owner, EAiStateType NextState)
	{
		static_assert(std::is_base_of_v<UAiState, T>, "T must derive from UAiState");

		T* State = NewObject<T>(Outer, T::StaticClass());
		State->InitState();
		State->SetOwner(Owner);
		State->SetNextState(NextState);
		return State;
	}

	
	
protected:
	
UPROPERTY()
	TMap<EAiStateType, UAiState*> m_AiStates;
	
UPROPERTY()
	UAiState* m_CurrentState = nullptr;
UPROPERTY()
	UAiState* m_PreviousState = nullptr;
UPROPERTY()
	AMonster* m_Owner = nullptr;
};
