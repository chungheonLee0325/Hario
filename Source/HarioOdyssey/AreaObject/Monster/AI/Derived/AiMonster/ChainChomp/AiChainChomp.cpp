// Fill out your copyright notice in the Description page of Project Settings.


#include "AiChainChomp.h"

#include "HarioOdyssey/AreaObject/Monster/AI/Base/BaseAiState.h"
#include "HarioOdyssey/AreaObject/Monster/AI/Derived/CommonState//AggroWait.h"


// Sets default values for this component's properties
UAiChainChomp::UAiChainChomp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


// Called when the game starts
void UAiChainChomp::BeginPlay()
{
	Super::BeginPlay();

	// ...
		
}


// Called every frame
void UAiChainChomp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UAiChainChomp::InitStatePool()
{
	Super::InitStatePool();

	//템플릿을 사용한 Factory 패턴으로 타입 안정성 보장)
	auto AggroWait = CreateState<UAggroWait>(this, m_Owner,EAiStateType::Attack);
	// Builder 패턴을 이용한 상태 설정 기능
	AggroWait->SetM_DetectRange(500.0f);
	AggroWait->SetM_WaitTime(3.0f);
	m_AiStates.Add(EAiStateType::Idle, AggroWait);
}
