// Fill out your copyright notice in the Description page of Project Settings.


#include "ChainChompLaunch.h"

#include "ChainChompPull.h"
#include "HarioOdyssey/AreaObject/Monster/Variants/NormalMonsters/ChainChomp/ChainChomp.h"
#include "HarioOdyssey/Contents/HarioGameMode.h"
#include "HarioOdyssey/Utility/GhostTrail.h"

UChainChompLaunch::UChainChompLaunch()
{
	// ToDo : Skill Param으로 넘겨받기
	SkillData.PrepareTime = 0.01f;
	SkillData.CastTime = 1.0f;
	SkillData.PostCastTime = 0.7f;
	SkillData.Cooldown = 0.0f;
	SkillData.CastRange = 3500.f;

	RotateTime = 0.7f;
	LaunchTime = 1.f;
}

void UChainChompLaunch::ClearTimer1()
{
	GetWorld()->GetTimerManager().ClearTimer(Handle1);
}

void UChainChompLaunch::OnPhaseChanged(ESkillPhase NewPhase)
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
			
			break;
		}
	case ESkillPhase::Casting:
		{
			//ToDo : Base Skill 로 빼기
			// Sound 재생
			m_GameMode->PlayPositionalSound(10010, m_ChainChomp->GetActorLocation());

			m_ChainChomp->MoveToLocation(m_TargetPos, LaunchTime, EMovementInterpolationType::EaseOutBounce);
			m_ChainChomp->IsDestructDmgAble = true;

			// 약한 참조로 this 캡처
			TWeakObjectPtr<UChainChompLaunch> WeakThis = this;
			GetWorld()->GetTimerManager().SetTimer(Handle1, [WeakThis]()
			{
				if (auto* StrongThis = WeakThis.Get())
				{
					FTransform SpawnTransform = StrongThis->m_ChainChomp->GetMesh()->GetComponentTransform();

					AGhostTrail* GhostTrail = StrongThis->GetWorld()->SpawnActor<AGhostTrail>(
						AGhostTrail::StaticClass(),
						SpawnTransform
					);

					if (GhostTrail)
					{
						GhostTrail->InitByMaterials(StrongThis->m_ChainChomp->GetMesh(),
						                            StrongThis->m_ChainChomp->GetGhostTrailMaterials());
					}
				}
			}, 0.01f, true);
			break;
		}
	case ESkillPhase::PostCast:
		{
			if (!m_ChainChomp || !m_Target) return;

			m_ChainChomp->IsDestructDmgAble = false;
			ClearTimer1();

			// Launch 완료, Return 시작
			break;
		}
	}
}

void UChainChompLaunch::UpdatePreparePhase(float DeltaTime)
{
	Super::UpdatePreparePhase(DeltaTime);
}

void UChainChompLaunch::UpdateCastingPhase(float DeltaTime)
{
	Super::UpdateCastingPhase(DeltaTime);
}

void UChainChompLaunch::UpdatePostCastPhase(float DeltaTime)
{
	Super::UpdatePostCastPhase(DeltaTime);
}

void UChainChompLaunch::BlockByObject()
{
	m_ChainChomp->IsDestructDmgAble = false;
	ClearTimer1();
}
