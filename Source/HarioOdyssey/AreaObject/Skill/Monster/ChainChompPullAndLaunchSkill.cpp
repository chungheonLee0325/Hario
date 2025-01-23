#include "ChainChompPullAndLaunchSkill.h"
#include "HarioOdyssey/AreaObject/Monster/BaseMonster.h"
#include "HarioOdyssey/AreaObject/Monster/Variants/NormalMonsters/ChainChomp/ChainChomp.h"
#include "HarioOdyssey/Contents/HarioGameInstance.h"
#include "HarioOdyssey/PathMover/VerticalMover.h"
#include "HarioOdyssey/Sound/SoundManager.h"
#include "Kismet/KismetMathLibrary.h"

UChainChompPullAndLaunchSkill::UChainChompPullAndLaunchSkill()
{
	SkillData.PrepareTime = 0.5f;
	SkillData.CastTime = 2.0f;
	SkillData.PostCastTime = 0.5f;
	SkillData.Cooldown = 3.0f;
	SkillData.CastRange = 1200.f;
}

bool UChainChompPullAndLaunchSkill::CanCast(ABaseMonster* Caster, const AActor* Target) const
{
	if (!Super::CanCast(Caster, Target)) return false;

	// 추가 체크 로직 (예: 체인 길이 제한 등)
	return true;
}

void UChainChompPullAndLaunchSkill::OnCastStart(ABaseMonster* Caster, const AActor* Target)
{
	//Super::OnCastStart(Caster, Target); 별도 로직이므로 x

	m_Target = Target;
	m_ChainChomp = Cast<AChainChomp>(Caster);

	if (!m_ChainChomp || !m_Target) return;

	Cast<UHarioGameInstance>(GetWorld()->GetGameInstance())->SoundManager->PlayPositionalSound(10010,m_ChainChomp->GetActorLocation());
	
	m_CurrentPhase = ESkillPhase::Prepare;
	m_OriginalPosition = m_ChainChomp->GetRootPosition();

	// ToDo : Skill Param으로 넘겨받기
	// 타겟 반대 방향으로 PullBackPosition 계산

	FVector Direction = (m_ChainChomp->GetActorLocation() - m_Target->GetActorLocation()).GetSafeNormal2D();
	m_PullBackPosition = m_ChainChomp->GetActorLocation() + Direction * PullBackDistance;
	m_TargetPos = m_ChainChomp->GetActorLocation() - Direction * PullBackDistance;

	// 타겟 주시
	m_ChainChomp->LookAtLocation(m_TargetPos, RotateTime * 0.5f);

	// 뒤로 당기기 시작
	m_ChainChomp->MoveToLocationWithSpeed(m_PullBackPosition, PullSpeed, EMovementInterpolationType::EaseOut);
}

void UChainChompPullAndLaunchSkill::OnCastTick(float DeltaTime)
{
	//Super::OnCastTick(Caster, Target, DeltaTime);

	if (!m_ChainChomp || !m_Target) return;

	switch (m_CurrentPhase)
	{
	case ESkillPhase::Prepare:
		// ToDo IsMoving 보다는 이벤트로 처리
		if (!m_ChainChomp->IsMoving())
		{
			// Pull 완료, Launch 시작
			m_CurrentPhase = ESkillPhase::Casting;
			m_ChainChomp->IsDestructDmgAble = true;
			m_ChainChomp->MoveToLocationWithSpeed(m_TargetPos, LaunchSpeed, EMovementInterpolationType::EaseOutBounce);
		}
		break;

	case ESkillPhase::Casting:
		// ToDo IsMoving 보다는 이벤트로 처리
		if (!m_ChainChomp->IsMoving())
		{
			// Launch 완료, Return 시작
			m_CurrentPhase = ESkillPhase::PostCast;
			m_ChainChomp->IsDestructDmgAble = false;
			m_ChainChomp->LookAtLocation(m_OriginalPosition, RotateTime, EMovementInterpolationType::Linear);
			m_ChainChomp->MoveToLocationWithSpeed(m_OriginalPosition, ReturnSpeed, EMovementInterpolationType::Linear);
			//m_ChainChomp->ReturnComponentToOriginal(m_ChainChomp->ChainChompRoot, 2.f);

			// 점프 시퀀스 시작
			PerformJumpSequence();
			
			// FTimerHandle TimerHandle;
			// TWeakObjectPtr<UChainChompPullAndLaunchSkill> WeakThis = this;
			// GetWorld()->GetTimerManager().SetTimer(TimerHandle, [WeakThis]()
			// {
			// 	UChainChompPullAndLaunchSkill* StrongThis = WeakThis.Get();
			// 	StrongThis->m_ChainChomp->m_VerticalMover->StartVerticalMovement(StrongThis->m_ChainChomp->GetMesh(), 200.f, 0.4f, 0.3f);
			// }, 0.3f, false);
			// FTimerHandle TimerHandle2;
			// GetWorld()->GetTimerManager().SetTimer(TimerHandle2, [this]()
			// {
			// 	m_ChainChomp->m_VerticalMover->StartVerticalMovement(m_ChainChomp->GetMesh(), 200.f, 0.4f, 0.3f);
			// 	m_ChainChomp->LookAtLocation(m_Target->GetActorLocation(), RotateTime, EMovementInterpolationType::Linear);
			// }, 1.05f, false);
		}
		break;

	case ESkillPhase::PostCast:
		// ToDo IsMoving 보다는 이벤트로 처리
		if (!m_ChainChomp->IsMoving())
		{
			FVector targetPos = FVector(m_Target->GetActorLocation().X, m_Target->GetActorLocation().Y,
			                            m_ChainChomp->GetActorLocation().Z);
			m_ChainChomp->LookAtLocation(targetPos, RotateTime * 0.5, EMovementInterpolationType::Linear);


			// Return 완료, 스킬 종료
			OnCastEnd();
		}
		break;
	}
}
void UChainChompPullAndLaunchSkill::PerformJumpSequence()
{
	// 약한 참조로 this 캡처
	TWeakObjectPtr<UChainChompPullAndLaunchSkill> WeakThis = this;

	// 첫 번째 점프
	GetWorld()->GetTimerManager().SetTimer(FirstJumpHandle, [WeakThis]()
	{
		if (auto* Skill = WeakThis.Get())
		{
			if (IsValid(Skill->m_ChainChomp) && Skill->m_ChainChomp->m_VerticalMover)
			{
				Skill->m_ChainChomp->m_VerticalMover->StartVerticalMovement(
					Skill->m_ChainChomp->GetMesh(), 
					Skill->JumpHeight, 
					Skill->JumpDuration, 
					Skill->LandingDuration
				);
			}
		}
	}, FirstJumpDelay, false);

	// 두 번째 점프와 회전
	GetWorld()->GetTimerManager().SetTimer(SecondJumpHandle, [WeakThis]()
	{
		if (auto* Skill = WeakThis.Get())
		{
			if (IsValid(Skill->m_ChainChomp) && Skill->m_ChainChomp->m_VerticalMover)
			{
				// 점프 실행
				Skill->m_ChainChomp->m_VerticalMover->StartVerticalMovement(
					Skill->m_ChainChomp->GetMesh(), 
					Skill->JumpHeight, 
					Skill->JumpDuration, 
					Skill->LandingDuration
				);

				// 타겟 방향으로 회전
				if (IsValid(Skill->m_Target))
				{
					Skill->m_ChainChomp->LookAtLocation(
						Skill->m_Target->GetActorLocation(), 
						Skill->RotateTime, 
						EMovementInterpolationType::Linear
					);
				}
			}
		}
	}, SecondJumpDelay, false);
}
void UChainChompPullAndLaunchSkill::OnCastEnd()
{
	//Super::OnCastEnd(Caster, Target);
	m_CurrentPhase = ESkillPhase::Ready;

	if (OnSkillComplete.IsBound() == true)
	{
		OnSkillComplete.Execute();
		OnSkillComplete.Unbind();
	}

	m_ChainChomp->ClearCurrentSkill();
}
