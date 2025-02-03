// Fill out your copyright notice in the Description page of Project Settings.


#include "VerticalMover.h"
UVerticalMover::UVerticalMover()
{
    PrimaryComponentTick.bCanEverTick = true;
    bIsMoving = false;
    CurrentTime = 0.0f;
    HoverTimer = 0.0f;
    TargetComponent =nullptr;
    CurrentMoveState = EMoveState::Stopped;
}

void UVerticalMover::BeginPlay()
{
    Super::BeginPlay();
}

void UVerticalMover::StartVerticalMovement(USceneComponent* Target, float InTargetHeight, float InRiseTime, float InFallTime)
{
    if (nullptr == Target || InRiseTime <= 0.0f || InFallTime <= 0.0f)
    {
        return;
    }

    TargetComponent = Target;
    InitialRelativeLocation = TargetComponent->GetRelativeLocation();
    TargetHeight = InTargetHeight;
    RiseTime = InRiseTime;
    FallTime = InFallTime;
    CurrentTime = 0.0f;
    HoverTimer = 0.0f;
    bIsMoving = true;
    CurrentMoveState = EMoveState::Rising;
}

void UVerticalMover::StopVerticalMovement()
{
    if (bIsMoving && TargetComponent && bAutoReturnToStart)
    {
        TargetComponent->SetRelativeLocation(InitialRelativeLocation);
    }
    
    bIsMoving = false;
    CurrentTime = 0.0f;
    HoverTimer = 0.0f;
    CurrentMoveState = EMoveState::Stopped;
}

float UVerticalMover::CalculateVerticalOffset(float Time) const
{
    float Progress = 0.0f;
    float CurrentOffset = 0.0f;

    switch (CurrentMoveState)
    {
        case EMoveState::Rising:
            Progress = FMath::Clamp(Time / RiseTime, 0.0f, 1.0f);
            // 이지아웃(Ease-out) 커브 적용: 시작할 때 빠르고 끝날 때 느려짐
            Progress = 1.0f - FMath::Pow(1.0f - Progress, 2.0f);
            CurrentOffset = TargetHeight * Progress;
            break;

        case EMoveState::Falling:
            Progress = FMath::Clamp(Time / FallTime, 0.0f, 1.0f);
            // 이지인(Ease-in) 커브 적용: 시작할 때 천천히, 끝날 때 빠르게
            Progress = Progress * Progress;
            CurrentOffset = TargetHeight * (1.0f - Progress);
            break;

        default:
            break;
    }

    return CurrentOffset;
}

void UVerticalMover::UpdateMovement(float DeltaTime)
{
    if (!TargetComponent || !bIsMoving)
        return;

    switch (CurrentMoveState)
    {
        case EMoveState::Rising:
            CurrentTime += DeltaTime;
            if (CurrentTime >= RiseTime)
            {
                CurrentTime = 0.0f;
                if (HoverTime > 0.0f)
                {
                    // 호버링 시작
                    HoverTimer = HoverTime;
                    FVector PeakLocation = InitialRelativeLocation;
                    PeakLocation.Z += TargetHeight;
                    TargetComponent->SetRelativeLocation(PeakLocation);
                    break;
                }
                else
                {
                    CurrentMoveState = EMoveState::Falling;
                }
            }
            break;

        case EMoveState::Falling:
            CurrentTime += DeltaTime;
            if (CurrentTime >= FallTime)
            {
                if (bAutoReturnToStart)
                {
                    TargetComponent->SetRelativeLocation(InitialRelativeLocation);
                }
                StopVerticalMovement();
                return;
            }
            break;

        default:
            break;
    }

    // 호버링 처리
    if (HoverTimer > 0.0f)
    {
        HoverTimer -= DeltaTime;
        if (HoverTimer <= 0.0f)
        {
            CurrentMoveState = EMoveState::Falling;
            CurrentTime = 0.0f;
        }
        return;
    }

    // 위치 업데이트
    FVector NewLocation = InitialRelativeLocation;
    NewLocation.Z += CalculateVerticalOffset(CurrentTime);
    TargetComponent->SetRelativeLocation(NewLocation);
}

void UVerticalMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    UpdateMovement(DeltaTime);
}