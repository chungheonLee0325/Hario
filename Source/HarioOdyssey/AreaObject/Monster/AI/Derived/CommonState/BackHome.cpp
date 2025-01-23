// Fill out your copyright notice in the Description page of Project Settings.


#include "BackHome.h"

#include "HarioOdyssey/AreaObject/Monster/AI/Base/BaseAiFSM.h"
#include "HarioOdyssey/AreaObject/Monster/Variants/NormalMonsters/ChainChomp/ChainChomp.h"

void UBackHome::InitState()
{
	BackHomeDuration = 2.0f;
}

void UBackHome::Enter()
{
	BackHomeElapsedTime = 0.f;
	auto location = m_Owner->GetM_SpawnLocation();
	m_Owner->LookAtLocation(location, 0.5f);
	m_Owner->MoveToLocation(location, BackHomeDuration);
}

void UBackHome::Execute(float dt)
{
	BackHomeElapsedTime += dt;
	if (BackHomeDuration <= BackHomeElapsedTime)
	{
		m_AiFSM->ChangeState(EAiStateType::Idle);
	}
}

void UBackHome::Exit()
{
}
