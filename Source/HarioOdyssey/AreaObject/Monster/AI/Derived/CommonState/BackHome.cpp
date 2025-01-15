// Fill out your copyright notice in the Description page of Project Settings.


#include "BackHome.h"

#include "HarioOdyssey/AreaObject/Monster/AI/Base/BaseAiFSM.h"

void UBackHome::InitState()
{
	Super::InitState();
}

void UBackHome::Enter()
{
	Super::Enter();
}

void UBackHome::Execute(float dt)
{
	Super::Execute(dt);
	auto prevLoc = m_RootObject->GetComponentLocation();
	auto dir = (m_HomePosition - prevLoc);
	if (dir.Length() < 0.1f)
	{
		m_AiFSM->ChangeState(m_NextState);
	}
	dir.Normalize();
	
	m_RootObject -> SetWorldLocation(prevLoc + dir * m_MovementSpeed * dt);
}

void UBackHome::Exit()
{
	Super::Exit();
}

void UBackHome::SetM_HomePosition(const FVector& M_HomePosition)
{
	m_HomePosition = M_HomePosition;
}

void UBackHome::SetM_RootObject(USceneComponent* M_RootComponent)
{
	m_RootObject = M_RootComponent;
}

void UBackHome::SetM_MovementSpeed(float M_MovementSpeed)
{
	m_MovementSpeed = M_MovementSpeed;
}
