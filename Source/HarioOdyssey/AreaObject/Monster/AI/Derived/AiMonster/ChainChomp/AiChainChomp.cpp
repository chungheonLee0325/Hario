// Fill out your copyright notice in the Description page of Project Settings.


#include "AiChainChomp.h"
#include "HarioOdyssey/AreaObject/Monster/Variants/NormalMonsters/ChainChomp.h"
#include "HarioOdyssey/AreaObject/Monster/AI/Derived/CommonState/AggroWait.h"
#include "HarioOdyssey/AreaObject/Monster/AI/Derived/CommonState/CommonAttack.h"
#include "HarioOdyssey/AreaObject/Monster/AI/Derived/CommonState/BackHome.h"


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
	AChainChomp* ChainChompOwner = Cast<AChainChomp>(m_Owner);
	if (!ChainChompOwner) return;
	
	m_OriginalPosition = m_Owner->GetActorLocation();

	// AggroWait 상태 설정
	auto AggroWait = CreateState<UAggroWait>(this, m_Owner, EAiStateType::Idle);
	AggroWait->SetM_DetectRange(ChainChompOwner->GetChainLength() + 50.0f);
	AggroWait->SetM_WaitTime(2.0f);
	AggroWait->SetNextState(EAiStateType::Attack);
	AddState(EAiStateType::Idle, AggroWait);

	// Attack 상태 설정
	auto Attack = CreateState<UCommonAttack>(this, m_Owner, EAiStateType::Attack);
	//Attack->SetM_PathMover(ChainChompOwner->ChainChompPathMover);
	Attack->SetM_AttackSpeed(ChainChompOwner->GetAttackSpeed());
	Attack->SetM_ChainLength(ChainChompOwner->GetChainLength());
	Attack->SetM_AnchorPosition(m_OriginalPosition);
	Attack->SetNextState(EAiStateType::Idle);
	AddState(EAiStateType::Attack, Attack);

	//// BackHome 상태 설정
	//auto BackHome = CreateState<UBackHome>(this, m_Owner, EAiStateType::Return);
	//BackHome->SetM_HomePosition(m_OriginalPosition);
	//BackHome->SetM_RootObject(ChainChompOwner->ChainChompRoot);
	//BackHome->SetM_MovementSpeed(ChainChompOwner->GetReturnSpeed());
	//BackHome->SetNextState(EAiStateType::Idle);
	//AddState(EAiStateType::Return, BackHome);

	ChangeState(EAiStateType::Idle);
}
