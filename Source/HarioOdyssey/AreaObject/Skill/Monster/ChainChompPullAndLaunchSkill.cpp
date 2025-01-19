#include "ChainChompPullAndLaunchSkill.h"
#include "HarioOdyssey/AreaObject/Monster/BaseMonster.h"
#include "HarioOdyssey/AreaObject/Monster/Variants/NormalMonsters/ChainChomp/ChainChomp.h"
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

    m_CurrentPhase = ESkillPhase::Prepare;
    m_OriginalPosition = m_ChainChomp->GetRootPosition();

    // ToDo : Skill Param으로 넘겨받기
    // 타겟 반대 방향으로 PullBackPosition 계산
    
    FVector Direction = (m_ChainChomp->GetActorLocation() - m_Target->GetActorLocation()).GetSafeNormal2D();
    m_PullBackPosition = m_ChainChomp->GetActorLocation()  + Direction * PullBackDistance;
    m_TargetPos = m_ChainChomp->GetActorLocation()  - Direction * PullBackDistance;

    // 타겟 주시
    m_ChainChomp->LookAtLocation(m_TargetPos, RotateTime);
    
    // 뒤로 당기기 시작
    m_ChainChomp->MoveToLocationWithSpeed(m_PullBackPosition, PullSpeed);
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
            m_ChainChomp->MoveToLocationWithSpeed(m_TargetPos, LaunchSpeed);
        }
        break;

    case ESkillPhase::Casting:
        // ToDo IsMoving 보다는 이벤트로 처리
        if (!m_ChainChomp->IsMoving())
        {
            // Launch 완료, Return 시작
            m_CurrentPhase = ESkillPhase::PostCast;
            m_ChainChomp->LookAtLocation(m_OriginalPosition, RotateTime, EMovementInterpolationType::EaseIn);
            m_ChainChomp->MoveToLocationWithSpeed(m_OriginalPosition, ReturnSpeed);
            //m_ChainChomp->ReturnComponentToOriginal(m_ChainChomp->ChainChompRoot, 2.f);
        }
        break;

    case ESkillPhase::PostCast:
        // ToDo IsMoving 보다는 이벤트로 처리
        if (!m_ChainChomp->IsMoving())
        {
            FVector targetPos = FVector(m_Target->GetActorLocation().X,m_Target->GetActorLocation().Y,m_ChainChomp->GetActorLocation().Z);
            m_ChainChomp->LookAtLocation(targetPos, RotateTime, EMovementInterpolationType::EaseIn);
            // Return 완료, 스킬 종료
            OnCastEnd();
        }
        break;
    }
}

void UChainChompPullAndLaunchSkill::OnCastEnd()
{
    //Super::OnCastEnd(Caster, Target);
    m_CurrentPhase = ESkillPhase::Ready;
} 