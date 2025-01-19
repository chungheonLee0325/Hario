// Fill out your copyright notice in the Description page of Project Settings.


#include "BackHome.h"

#include "HarioOdyssey/AreaObject/Monster/AI/Base/BaseAiFSM.h"
#include "HarioOdyssey/AreaObject/Monster/Variants/NormalMonsters/ChainChomp/ChainChomp.h"

void UBackHome::InitState()
{
}

void UBackHome::Enter()
{
}

void UBackHome::Execute(float dt)
{
}

void UBackHome::Exit()
{
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
