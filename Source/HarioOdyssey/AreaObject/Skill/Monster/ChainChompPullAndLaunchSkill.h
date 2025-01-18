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
    float PullBackDistance = 900.f;

    UPROPERTY(EditAnywhere, Category = "Skill Settings")
    float PullSpeed = 1500.f;

public:
    void SetPullBackDistance(float _PullBackDistance)
    {
        this->PullBackDistance = _PullBackDistance;
    }

    void SetPullSpeed(float _PullSpeed)
    {
        this->PullSpeed = _PullSpeed;
    }

    void SetLaunchSpeed(float _LaunchSpeed)
    {
        this->LaunchSpeed = _LaunchSpeed;
    }

    void SetReturnSpeed(float _ReturnSpeed)
    {
        this->ReturnSpeed = _ReturnSpeed;
    }

    void SetRotateTime(float _RotateTime)
    {
        this->RotateTime = _RotateTime;
    }

    void SetM_ChainChomp(AChainChomp* _M_ChainChomp)
    {
        m_ChainChomp = _M_ChainChomp;
    }

protected:
    UPROPERTY(EditAnywhere, Category = "Skill Settings")
    float LaunchSpeed = 3000.f;

    UPROPERTY(EditAnywhere, Category = "Skill Settings")
    float ReturnSpeed = 500.f;

    UPROPERTY(EditAnywhere, Category = "Skill Settings")
    float RotateTime = 1.0f;
private:
    FVector m_PullBackPosition;
    FVector m_OriginalPosition;

UPROPERTY(EditAnywhere)
    AChainChomp * m_ChainChomp;
}; 