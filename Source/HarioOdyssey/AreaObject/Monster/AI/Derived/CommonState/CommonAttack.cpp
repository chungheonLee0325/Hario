// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonAttack.h"
#include "HarioOdyssey/AreaObject/Monster/BaseMonster.h"
#include "HarioOdyssey/AreaObject/Monster/AI/Base/BaseAiFSM.h"
#include "HarioOdyssey/AreaObject/Skill/Base/BaseSkill.h"

void UCommonAttack::InitState()
{
	//m_AiStateType = EAiStateType::Attack;
}

void UCommonAttack::Enter()
{
	m_IsSkillStarted = false;
}

void UCommonAttack::Execute(float DeltaTime)
{
	if (!m_Owner) return;

	// 타겟 체크
	AActor* Target = m_Owner->GetAggroTarget();
	if (!Target)
	{
		m_AiFSM->ChangeState(EAiStateType::Idle);
		return;
	}
	if (!m_IsSkillStarted)
	{
		// 공격 스킬 시작
		if (UBaseSkill* AttackSkill = m_Owner->GetSkillByState(m_AiStateType))
		{
			if (m_Owner->CanCastSkill(AttackSkill,Target))
			{
				m_Owner->CastSkill(AttackSkill,Target);
				m_IsSkillStarted = true;
			}
			m_Owner->CastSkill(AttackSkill, m_Owner->GetAggroTarget());
		}
		else
		{
			m_AiFSM->ChangeState(EAiStateType::Idle);
		}
	}
	else
	{
		if (UBaseSkill* AttackSkill = m_Owner->GetSkillByState(m_AiStateType))
		{
			if (AttackSkill->GetCurrentPhase() == ESkillPhase::Ready)
			{
				m_AiFSM->ChangeState(m_NextState);
			}
		}
	}
	//// 현재 페이즈에 따른 실행
	//switch (m_CurrentPhase)
	//{
	//case EAttackPhase::Prepare:
	//	ExecutePreparePhase(DeltaTime);
	//	break;
	//case EAttackPhase::Attack:
	//	ExecuteAttackPhase(DeltaTime);
	//	break;
	//case EAttackPhase::End:
	//	ExecuteEndPhase(DeltaTime);
	//	break;
	//}
}

void UCommonAttack::Exit()
{
	if (!m_Owner) return;

	// 공격 스킬 종료
	if (UBaseSkill* AttackSkill = m_Owner->GetSkillByState(m_AiStateType))
	{
		AttackSkill->OnCastEnd(m_Owner, m_Owner->GetAggroTarget());
	}
}

void UCommonAttack::ExecutePreparePhase(float DeltaTime)
{
	const float PrepareTime = 0.5f;
	m_PhaseTimer += DeltaTime;

	// 타겟 방향으로 회전
	if (AActor* Target = m_Owner->GetAggroTarget())
	{
		m_Owner->LookAtLocationWithSpeed(Target->GetActorLocation(), m_Owner->MonsterData.RotationSpeed);

		// 약간 뒤로 당기기
		FVector PullBackDir = (m_AnchorPosition - m_Owner->GetActorLocation()).GetSafeNormal();
		m_Owner->MoveToLocationWithSpeed(
			m_Owner->GetActorLocation() + PullBackDir * 100.0f, // 뒤로 당길 거리
			m_AttackSpeed * 0.5f
		);
	}

	// 준비 시간이 끝나면 공격 페이즈로
	if (m_PhaseTimer >= PrepareTime)
	{
		m_CurrentPhase = EAttackPhase::Attack;
		m_PhaseTimer = 0.0f;
	}
}

void UCommonAttack::ExecuteAttackPhase(float DeltaTime)
{
	if (AActor* Target = m_Owner->GetAggroTarget())
	{
		// 사정거리 체크
		if (IsInAttackRange(Target->GetActorLocation()))
		{
			// 타겟을 향해 돌진
			m_Owner->MoveToLocationWithSpeed(Target->GetActorLocation(), m_AttackSpeed);
		}
		else
		{
			// 사정거리를 벗어나면 종료
			m_CurrentPhase = EAttackPhase::End;
			m_PhaseTimer = 0.0f;
		}
	}
}

void UCommonAttack::ExecuteEndPhase(float DeltaTime)
{
	const float EndTime = 0.3f;
	m_PhaseTimer += DeltaTime;

	if (m_PhaseTimer >= EndTime)
	{
		m_AiFSM->ChangeState(m_NextState);
	}
}

bool UCommonAttack::IsInAttackRange(const FVector& TargetLocation) const
{
	if (!m_Owner) return false;

	float DistanceToTarget = FVector::Distance(m_Owner->GetActorLocation(), TargetLocation);
	float DistanceToAnchor = FVector::Distance(m_Owner->GetActorLocation(), m_AnchorPosition);

	// 최소/최대 사정거리와 체인 길이 모두 체크
	return DistanceToTarget >= m_MinAttackRange &&
		DistanceToTarget <= m_MaxAttackRange &&
		DistanceToAnchor <= m_ChainLength;
}

