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
    float PullBackDistance = 400.f;

    UPROPERTY(EditAnywhere, Category = "Skill Settings")
    float PullSpeed = 300.f;

    UPROPERTY(EditAnywhere, Category = "Skill Settings")
    float LaunchSpeed = 1300.f;

    UPROPERTY(EditAnywhere, Category = "Skill Settings")
    float ReturnSpeed = 150.f;

    UPROPERTY(EditAnywhere, Category = "Skill Settings")
    float RotateTime = 1.0f;
private:
    FVector m_PullBackPosition;
    FVector m_OriginalPosition;

UPROPERTY(EditAnywhere)
    AChainChomp * m_ChainChomp;
}; 