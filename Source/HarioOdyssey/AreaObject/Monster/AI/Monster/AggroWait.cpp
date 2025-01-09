// Fill out your copyright notice in the Description page of Project Settings.


#include "AggroWait.h"




void UAggroWait::InitState()
{
	Super::InitState();
}

void UAggroWait::Enter()
{
	Super::Enter();
}

void UAggroWait::Execute(float dt)
{
	Super::Execute(dt);
}

void UAggroWait::Exit()
{
	Super::Exit();
}
void UAggroWait::SetDetectRange(float Range)
{
	m_Range = Range;
}

void UAggroWait::SetWaitTime(float Time)
{
	m_Time = Time;
}