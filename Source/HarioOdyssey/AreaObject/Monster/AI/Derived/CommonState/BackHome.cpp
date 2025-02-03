// Fill out your copyright notice in the Description page of Project Settings.


#include "BackHome.h"

#include "HarioOdyssey/AreaObject/Monster/AI/Base/BaseAiFSM.h"
#include "HarioOdyssey/AreaObject/Monster/Variants/NormalMonsters/ChainChomp/ChainChomp.h"
#include "HarioOdyssey/PathMover/VerticalMover.h"

void UBackHome::InitState()
{
	BackHomeDuration = 1.5f;
}

void UBackHome::Enter()
{
	BackHomeElapsedTime = 0.f;
	auto location = m_Owner->GetM_SpawnLocation();
	location.Z = m_Owner->GetActorLocation().Z;
	float t = FVector::Dist2D(location, m_Owner->GetActorLocation());
	UE_LOG(LogTemp, Warning, TEXT("UBackHome::Enter distance : %f"), t);


	m_Owner->LookAtLocation(location, 0.5f);
	if (t > 800.f)
	{
		m_Owner->MoveToLocation(location, BackHomeDuration);
		PerformJumpSequence();
	}
	else
	{
		float delta = 900 / t;
		m_Owner->MoveToLocation(location, BackHomeDuration / delta);
		m_Owner->m_VerticalMover->StartVerticalMovement(
			m_Owner->GetMesh(),
			JumpHeight,
			BackHomeDuration * 0.7 / delta,
			BackHomeDuration * 0.3 / delta
		);
		m_Owner->LookAtLocation(
			m_Owner->GetAggroTarget()->GetActorLocation(),
			RotateTime,
			EMovementInterpolationType::Linear);
	}
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

void UBackHome::PerformJumpSequence()
{
	// 약한 참조로 this 캡처
	TWeakObjectPtr<UBackHome> WeakThis = this;

	m_Owner->m_VerticalMover->StartVerticalMovement(
					m_Owner->GetMesh(),
					JumpHeight,
					BackHomeDuration * 0.35,
					BackHomeDuration * 0.15);

	// 두 번째 점프와 회전
	GetWorld()->GetTimerManager().SetTimer(SecondJumpHandle, [WeakThis]()
	{
		if (auto* Skill = WeakThis.Get())
		{
			if (auto* StrongThis = WeakThis.Get())
			{
				if (StrongThis->m_Owner->m_VerticalMover != nullptr)
				{
					StrongThis->m_Owner->m_VerticalMover->StartVerticalMovement(
						StrongThis->m_Owner->GetMesh(),
						StrongThis->JumpHeight,
						StrongThis->BackHomeDuration * 0.35,
						StrongThis->BackHomeDuration * 0.15
					);

					// 타겟 방향으로 회전
					StrongThis->m_Owner->LookAtLocation(
						StrongThis->m_Owner->GetAggroTarget()->GetActorLocation(),
						StrongThis->RotateTime,
						EMovementInterpolationType::Linear
					);
				}
			}
		}
	}, BackHomeDuration / 2 + 0.01f, false);
}
