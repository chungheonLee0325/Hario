#include "ChainChompPullAndLaunchSkill.h"
#include "HarioOdyssey/AreaObject/Monster/BaseMonster.h"

UChainChompPullAndLaunchSkill::UChainChompPullAndLaunchSkill()
{
    SkillData.PrepareTime = 0.5f;
    SkillData.CastTime = 2.0f;
    SkillData.PostCastTime = 0.5f;
    SkillData.Cooldown = 3.0f;
    SkillData.CastRange = 1000.f;
}

bool UChainChompPullAndLaunchSkill::CanCast(ABaseMonster* Caster, const AActor* Target) const
{
    if (!Super::CanCast(Caster, Target)) return false;

    // 추가 체크 로직 (예: 체인 길이 제한 등)
    return true;
}

void UChainChompPullAndLaunchSkill::OnCastStart(ABaseMonster* Caster, const AActor* Target)
{
    //Super::OnCastStart(Caster, Target);

    if (!Caster || !Target) return;

    m_CurrentPhase = ESkillPhase::Prepare;
    m_OriginalPosition = Caster->GetActorLocation();

    // ToDo : Skill Param으로 넘겨받기
    // 타겟 반대 방향으로 PullBackPosition 계산
    FVector Direction = (Caster->GetActorLocation() - Target->GetActorLocation()).GetSafeNormal();
    m_PullBackPosition = Caster->GetActorLocation() + Direction * PullBackDistance;

    // 뒤로 당기기 시작
    Caster->MoveToLocationWithSpeed(m_PullBackPosition, PullSpeed);
}

void UChainChompPullAndLaunchSkill::OnCastTick(ABaseMonster* Caster, const AActor* Target, float DeltaTime)
{
    //Super::OnCastTick(Caster, Target, DeltaTime);

    if (!Caster || !Target) return;

    switch (m_CurrentPhase)
    {
    case ESkillPhase::Prepare:
        // ToDo IsMoving 보다는 이벤트로 처리
        if (!Caster->IsMoving())
        {
            // Pull 완료, Launch 시작
            m_CurrentPhase = ESkillPhase::Casting;
            Caster->MoveToActorWithSpeed(Target, LaunchSpeed);
        }
        break;

    case ESkillPhase::Casting:
        // ToDo IsMoving 보다는 이벤트로 처리
        if (!Caster->IsMoving())
        {
            // Launch 완료, Return 시작
            m_CurrentPhase = ESkillPhase::PostCast;
            Caster->MoveToLocationWithSpeed(m_OriginalPosition, ReturnSpeed);
        }
        break;

    case ESkillPhase::PostCast:
        // ToDo IsMoving 보다는 이벤트로 처리
        if (!Caster->IsMoving())
        {
            // Return 완료, 스킬 종료
            OnCastEnd(Caster, Target);
        }
        break;
    }
}

void UChainChompPullAndLaunchSkill::OnCastEnd(ABaseMonster* Caster, const AActor* Target)
{
    //Super::OnCastEnd(Caster, Target);
    m_CurrentPhase = ESkillPhase::Ready;
} 