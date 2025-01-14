// Fill out your copyright notice in the Description page of Project Settings.


#include "PathMover.h"

UPathMover::UPathMover()
{
    PrimaryComponentTick.bCanEverTick = true;
    bIsMoving = false;
    bIsRotating = false;
    CurrentLerpTime = 0.0f;
    MovementDuration = 0.0f;
    RotationSpeed = 5.0f;
}

void UPathMover::BeginPlay()
{
    Super::BeginPlay();
}

void UPathMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bIsMoving)
    {
        UpdateMovement(DeltaTime);
    }

    if (bIsRotating)
    {
        UpdateRotation(DeltaTime);
    }
}

void UPathMover::MoveComponentTo(USceneComponent* ComponentToMove, const FVector& _TargetPosition, 
                                float Duration, EMovementInterpolationType InterpType)
{
    if (!ComponentToMove || Duration <= 0.0f) return;

    // 원래 위치 저장 (처음 이동하는 경우에만)
    if (!OriginalPositions.Contains(ComponentToMove))
    {
        OriginalPositions.Add(ComponentToMove, ComponentToMove->GetRelativeLocation());
    }

    StartNewMovement(ComponentToMove, _TargetPosition, Duration, InterpType);
}

void UPathMover::MoveActorTo(const FVector& _TargetPosition, float Duration, 
                             EMovementInterpolationType InterpType)
{
    if (!GetOwner() || Duration <= 0.0f) return;

    StartNewMovement(nullptr, _TargetPosition, Duration, InterpType);
}

void UPathMover::StartNewMovement(USceneComponent* ComponentToMove, const FVector& Target, 
                                 float Duration, EMovementInterpolationType InterpType)
{
    CurrentMovingComponent = ComponentToMove;
    StartPosition = ComponentToMove ? 
        ComponentToMove->GetComponentLocation() : 
        GetOwner()->GetActorLocation();
    TargetPosition = Target;
    MovementDuration = Duration;
    CurrentInterpType = InterpType;
    CurrentLerpTime = 0.0f;
    bIsMoving = true;
}

void UPathMover::UpdateMovement(float DeltaTime)
{
    CurrentLerpTime += DeltaTime;
    float Alpha = FMath::Clamp(CurrentLerpTime / MovementDuration, 0.0f, 1.0f);
    Alpha = CalculateInterpolationAlpha(Alpha, CurrentInterpType);

    FVector NewPosition = FMath::Lerp(StartPosition, TargetPosition, Alpha);

    if (CurrentMovingComponent.IsValid())
    {
        if (USceneComponent* Parent = CurrentMovingComponent->GetAttachParent())
        {
            FVector RelativePos = Parent->GetComponentTransform().InverseTransformPosition(NewPosition);
            CurrentMovingComponent->SetRelativeLocation(RelativePos);
        }
        else
        {
            CurrentMovingComponent->SetWorldLocation(NewPosition);
        }
    }
    else if (AActor* Owner = GetOwner())
    {
        Owner->SetActorLocation(NewPosition);
    }

    if (Alpha >= 1.0f)
    {
        bIsMoving = false;
        OnMovementCompleted.Broadcast();
    }
}

void UPathMover::RotateComponentToward(USceneComponent* ComponentToRotate, 
                                      const FVector& TargetPos, float InterpSpeed)
{
    if (!ComponentToRotate) return;

    CurrentMovingComponent = ComponentToRotate;
    RotationTarget = TargetPos;
    RotationSpeed = InterpSpeed;
    bIsRotating = true;
}

void UPathMover::RotateActorToward(const FVector& TargetPos, float InterpSpeed)
{
    if (!GetOwner()) return;

    CurrentMovingComponent = nullptr;
    RotationTarget = TargetPos;
    RotationSpeed = InterpSpeed;
    bIsRotating = true;
}

void UPathMover::UpdateRotation(float DeltaTime)
{
    if (!bIsRotating) return;

    FVector CurrentLocation;
    FRotator CurrentRotation;
    
    // 현재 위치와 회전값 가져오기
    if (CurrentMovingComponent.IsValid())
    {
        CurrentLocation = CurrentMovingComponent->GetComponentLocation();
        CurrentRotation = CurrentMovingComponent->GetComponentRotation();
    }
    else if (AActor* Owner = GetOwner())
    {
        CurrentLocation = Owner->GetActorLocation();
        CurrentRotation = Owner->GetActorRotation();
    }
    else
    {
        bIsRotating = false;
        return;
    }

    // 목표를 향하는 방향 계산
    FVector DirectionToTarget = (RotationTarget - CurrentLocation).GetSafeNormal();
    FRotator TargetRotation = DirectionToTarget.Rotation();

    // 현재 회전과 목표 회전 사이의 차이 계산
    float DeltaYaw = FMath::FindDeltaAngleDegrees(CurrentRotation.Yaw, TargetRotation.Yaw);
    
    // 회전이 거의 완료되었는지 확인 (5도 이내)
    if (FMath::Abs(DeltaYaw) < 5.0f)
    {
        // 정확한 목표 회전값으로 설정
        if (CurrentMovingComponent.IsValid())
        {
            CurrentMovingComponent->SetWorldRotation(FRotator(0.0f, TargetRotation.Yaw, 0.0f));
        }
        else if (AActor* Owner = GetOwner())
        {
            Owner->SetActorRotation(FRotator(0.0f, TargetRotation.Yaw, 0.0f));
        }
        
        bIsRotating = false;
        OnRotationCompleted.Broadcast();
        return;
    }

    // 부드러운 회전 적용
    FRotator NewRotation = FRotator(0.0f, 
        FMath::FInterpTo(CurrentRotation.Yaw, TargetRotation.Yaw, DeltaTime, RotationSpeed), 
        0.0f);

    // 회전 적용
    if (CurrentMovingComponent.IsValid())
    {
        CurrentMovingComponent->SetWorldRotation(NewRotation);
    }
    else if (AActor* Owner = GetOwner())
    {
        Owner->SetActorRotation(NewRotation);
    }
}

void UPathMover::ReturnComponentToOriginal(USceneComponent* ComponentToReturn, 
                                          float Duration, EMovementInterpolationType InterpType)
{
    if (!ComponentToReturn || Duration <= 0.0f) return;

    FVector* OriginalPos = OriginalPositions.Find(ComponentToReturn);
    if (!OriginalPos) return;

    if (USceneComponent* Parent = ComponentToReturn->GetAttachParent())
    {
        FVector WorldPos = Parent->GetComponentTransform().TransformPosition(*OriginalPos);
        MoveComponentTo(ComponentToReturn, WorldPos, Duration, InterpType);
    }
    else
    {
        MoveComponentTo(ComponentToReturn, *OriginalPos, Duration, InterpType);
    }
}

float UPathMover::CalculateInterpolationAlpha(float RawAlpha, EMovementInterpolationType InterpType) const
{
    switch (InterpType)
    {
        case EMovementInterpolationType::Linear:
            return RawAlpha;
        case EMovementInterpolationType::EaseIn:
            return RawAlpha * RawAlpha;
        case EMovementInterpolationType::EaseOut:
            return 1.0f - FMath::Square(1.0f - RawAlpha);
        case EMovementInterpolationType::EaseInOut:
            return RawAlpha < 0.5f ? 
                2.0f * RawAlpha * RawAlpha : 
                1.0f - FMath::Pow(-2.0f * RawAlpha + 2.0f, 2.0f) / 2.0f;
        default:
            return RawAlpha;
    }
} 