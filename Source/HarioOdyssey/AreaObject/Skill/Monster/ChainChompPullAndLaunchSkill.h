#pragma once

#include "CoreMinimal.h"
#include "HarioOdyssey/AreaObject/Skill/Base/BaseSkill.h"
#include "ChainChompPullAndLaunchSkill.generated.h"

class AChainChomp;

UCLASS()
class HARIOODYSSEY_API UChainChompPullAndLaunchSkill : public UBaseSkill
{
    GENERATED_BODY()

public:
    UChainChompPullAndLaunchSkill();

    virtual bool CanCast(ABaseMonster* Caster, const AActor* Target) const override;
    virtual void OnCastStart(ABaseMonster* Caster, const AActor* Target) override;
    virtual void OnCastTick(float DeltaTime) override;
    virtual void OnCastEnd() override;

protected:
    UPROPERTY(EditAnywhere, Category = "Skill Settings")
    float PullBackDistance = 200.f;

    UPROPERTY(EditAnywhere, Category = "Skill Settings")
    float PullSpeed = 500.f;

    UPROPERTY(EditAnywhere, Category = "Skill Settings")
    float LaunchSpeed = 1000.f;

    UPROPERTY(EditAnywhere, Category = "Skill Settings")
    float ReturnSpeed = 300.f;

private:
    FVector m_PullBackPosition;
    FVector m_OriginalPosition;

UPROPERTY(EditAnywhere)
    AChainChomp * m_ChainChomp;
}; 