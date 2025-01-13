// Fill out your copyright notice in the Description page of Project Settings.


#include "AggroWait.h"

#include "GameFramework/Character.h"
#include "HarioOdyssey/AreaObject/Base/AreaObject.h"
#include "HarioOdyssey/AreaObject/Monster/Monster.h"
#include "HarioOdyssey/AreaObject/Monster/AI/Base/AiFSM.h"
#include "Kismet/GameplayStatics.h"



void UAggroWait::InitState()
{
	Super::InitState();
}

void UAggroWait::Enter()
{
	Super::Enter();
	// 타겟 초기화
	m_Target = Cast<AAreaObject>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	// 타이머 초기화
	m_DetectCycleTimeElapsed = 0.0f;
	m_WaitTimeElapsed = 0.0f;
}

void UAggroWait::Execute(float dt)
{
	Super::Execute(dt);
	m_DetectCycleTimeElapsed += dt;

	// 매틱마다 검사보다는 정해진 시간마다 검사
	if (m_DetectCycleTimeElapsed < m_DetectCycleTime)
	{
		return;
	}
	m_DetectCycleTimeElapsed = 0.0f; // 검사 주기 타이머 초기화

	// 타겟 확인 및 탐지 검사
	if (nullptr == m_Target || false == DetectPlayer())
	{
		// 탐지 실패 시 대기 시간 초기화
		m_WaitTimeElapsed = 0.0f;
		return;
	}

	// 탐지 성공: 상태 전환 대기 시간 계산
	m_WaitTimeElapsed += dt;
	if (m_WaitTimeElapsed >= m_WaitTime)
	{
		m_AiFSM->ChangeState(m_NextAiState); // 상태 전환
	}
}

void UAggroWait::Exit()
{
	Super::Exit();
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
