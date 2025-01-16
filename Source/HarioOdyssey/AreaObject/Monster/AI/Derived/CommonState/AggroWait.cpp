// Fill out your copyright notice in the Description page of Project Settings.


#include "AggroWait.h"

#include "GameFramework/Character.h"
#include "HarioOdyssey/AreaObject/Base/AreaObject.h"
#include "HarioOdyssey/AreaObject/Monster/BaseMonster.h"
#include "HarioOdyssey/AreaObject/Monster/AI/Base/BaseAiFSM.h"
#include "HarioOdyssey/AreaObject/Monster/Variants/NormalMonsters/ChainChomp.h"
#include "Kismet/GameplayStatics.h"



void UAggroWait::InitState()
{
}

void UAggroWait::Enter()
{
	if (m_Target == nullptr)
	{
		m_Target = Cast<AAreaObject>( UGameplayStatics::GetPlayerPawn(this,0));
	}
	m_WaitTimeElapsed = 0.0f;
}

void UAggroWait::Execute(float dt)
{
	if (m_Owner == nullptr)
		return;

	m_WaitTimeElapsed += dt;

	
	if (DetectPlayer())
	{
		m_Owner->SetAggroTarget(m_Target);

		m_WaitTimeElapsed += dt;
		if (m_WaitTimeElapsed >= m_WaitTime)
		{
			m_AiFSM->ChangeState(m_NextState);
		}
	}
	else
	{
		m_Owner->SetAggroTarget(nullptr);
		m_WaitTimeElapsed = 0.0f;
	}
}

void UAggroWait::Exit()
{
}
void UAggroWait::SetM_DetectRange(float Range)
{
	m_DetectRange = Range;
}

void UAggroWait::SetM_WaitTime(float Time)
{
	m_WaitTime = Time;
}

bool UAggroWait::DetectPlayer() const
{
	if (m_Target->GetDistanceTo(m_Owner) <= m_DetectRange)
	{
		return true;
	}
	return false;
}
