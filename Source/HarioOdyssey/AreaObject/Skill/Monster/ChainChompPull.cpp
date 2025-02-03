// Fill out your copyright notice in the Description page of Project Settings.


#include "ChainChompPull.h"

#include "ChainChompLaunch.h"
#include "HarioOdyssey/AreaObject/Monster/Variants/NormalMonsters/ChainChomp/ChainChomp.h"
#include "HarioOdyssey/Contents/HarioGameMode.h"

UChainChompPull::UChainChompPull()
{
	// ToDo : Skill Param으로 넘겨받기
	SkillData.PrepareTime = 0.8f;
	SkillData.CastTime = 1.5f;
	SkillData.PostCastTime = 1.f;
	SkillData.Cooldown = 0.0f;
	SkillData.CastRange = 1500.f;

	PullBackDistance = 900.f;
	RotateTime = 0.7f;
	PullTime = 1.5f;

	// 스킬 예약 클래스
	m_NextSkillClass = UChainChompLaunch::StaticClass();
}

void UChainChompPull::OnPhaseChanged(ESkillPhase NewPhase)
{
	switch (NewPhase)
	{
	case ESkillPhase::Ready:
		{
			break;
		}
	case ESkillPhase::Prepare:
		{
			m_GameMode = Cast<AHarioGameMode>(GetWorld()->GetAuthGameMode());
			m_ChainChomp = Cast<AChainChomp>(m_Caster);

			if (!m_ChainChomp || !m_Target) return;
			
			// 스킬 예약 인스턴스
			m_NextSkill = m_ChainChomp->FindSkillByClass(m_NextSkillClass);

			m_OriginalPosition = m_ChainChomp->GetRootPosition();

			// 타겟 반대 방향으로 PullBackPosition 계산
			FVector Direction = (m_ChainChomp->GetActorLocation() - m_Target->GetActorLocation()).GetSafeNormal2D();
			m_PullBackPosition = m_ChainChomp->GetActorLocation() + Direction * PullBackDistance;
			m_TargetPos = m_ChainChomp->GetActorLocation() - Direction * PullBackDistance;

			// Next Skill Target 설정
			m_NextSkill->Set_TargetPos(m_TargetPos);
			
			// 타겟 주시
			m_ChainChomp->LookAtLocation(m_TargetPos, RotateTime * 0.5f);
			
			break;
		}
	case ESkillPhase::Casting:
		{
			if (!m_ChainChomp || !m_Target) return;

			// 뒤로 당기기 시작
			m_ChainChomp->MoveToLocation(m_PullBackPosition, PullTime, EMovementInterpolationType::EaseOut);
			break;
		}
	case ESkillPhase::PostCast:
		{
			if (!m_ChainChomp || !m_Target) return;
			// ToDo 마테리얼 변경
			m_ChainChomp->StartVibration(15.f,1.f,0.03f);
			break;
		}
	}
}

void UChainChompPull::UpdatePreparePhase(float DeltaTime)
{
	Super::UpdatePreparePhase(DeltaTime);
}

void UChainChompPull::UpdateCastingPhase(float DeltaTime)
{
	Super::UpdateCastingPhase(DeltaTime);
}

void UChainChompPull::UpdatePostCastPhase(float DeltaTime)
{
	Super::UpdatePostCastPhase(DeltaTime);
}
