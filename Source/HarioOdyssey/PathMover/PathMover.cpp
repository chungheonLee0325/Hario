// Fill out your copyright notice in the Description page of Project Settings.


#include "PathMover.h"

UPathMover::UPathMover()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UPathMover::BeginPlay()
{
    Super::BeginPlay();
}

void UPathMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (MovementState.bIsActive)
    {
        UpdateMovement(DeltaTime);
    }

    if (RotationState.bIsActive)
    {
        UpdateRotation(DeltaTime);
    }
}

void UPathMover::MoveComponentTo(USceneComponent* ComponentToMove, const FVector& TargetPosition, 
                                float Duration, EMovementInterpolationType InterpType)
{
    if (!ComponentToMove || Duration <= 0.0f) return;

    if (!OriginalPositions.Contains(ComponentToMove))
    {
        OriginalPositions.Add(ComponentToMove, ComponentToMove->GetRelativeLocation());
    }

    StartNewMovement(ComponentToMove, TargetPosition, Duration, InterpType);
}

void UPathMover::MoveActorTo(const FVector& TargetPosition, float Duration, 
                             EMovementInterpolationType InterpType)
{
    if (!GetOwner() || Duration <= 0.0f) return;
    StartNewMovement(nullptr, TargetPosition, Duration, InterpType);
}

void UPathMover::StartNewMovement(USceneComponent* ComponentToMove, const FVector& Target, 
                                 float Duration, EMovementInterpolationType InterpType)
{
    StopMovement(); // 이전 이동 중지

    CurrentMovingComponent = ComponentToMove;
    MovementState.StartPos = ComponentToMove ? 
        ComponentToMove->GetComponentLocation() : 
        GetOwner()->GetActorLocation();
    MovementState.TargetPos = Target;
    MovementState.Duration = Duration;
    MovementState.InterpType = InterpType;
    MovementState.CurrentTime = 0.0f;
    MovementState.bIsActive = true;
}

void UPathMover::StopMovement()
{
    if (!MovementState.bIsActive) return;

    // 현재 위치에서 정지
    if (CurrentMovingComponent.IsValid())
    {
        FVector CurrentPos = CurrentMovingComponent->GetComponentLocation();
        CurrentMovingComponent->SetWorldLocation(CurrentPos);
    }
    else if (AActor* Owner = GetOwner())
    {
        FVector CurrentPos = Owner->GetActorLocation();
        Owner->SetActorLocation(CurrentPos);
    }

    MovementState = FMovementState();
    ResetComponentReference();
    OnMovementCompleted.Broadcast();
}

void UPathMover::StopRotation()
{
    if (!RotationState.bIsActive) return;

    if (CurrentMovingComponent.IsValid())
    {
        FRotator CurrentRot = CurrentMovingComponent->GetComponentRotation();
        CurrentMovingComponent->SetWorldRotation(CurrentRot);
    }
    else if (AActor* Owner = GetOwner())
    {
        FRotator CurrentRot = Owner->GetActorRotation();
        Owner->SetActorRotation(CurrentRot);
    }

    RotationState = FRotationState();
    ResetComponentReference();
    OnRotationCompleted.Broadcast();
}

void UPathMover::ResetComponentReference()
{
    if (!MovementState.bIsActive && !RotationState.bIsActive)
    {
        CurrentMovingComponent.Reset();
    }
}

void UPathMover::UpdateMovement(float DeltaTime)
{
    MovementState.CurrentTime += DeltaTime;
    float Alpha = FMath::Clamp(MovementState.CurrentTime / MovementState.Duration, 0.0f, 1.0f);
    Alpha = CalculateInterpolationAlpha(Alpha, MovementState.InterpType);

    FVector NewPosition = FMath::Lerp(MovementState.StartPos, MovementState.TargetPos, Alpha);

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
        MovementState.bIsActive = false;
        OnMovementCompleted.Broadcast();
    }
}

void UPathMover::RotateComponentToward(USceneComponent* ComponentToRotate, 
                                      const FVector& TargetPos, float InterpSpeed)
{
    if (!ComponentToRotate) return;

    CurrentMovingComponent = ComponentToRotate;
    RotationState.TargetPos = TargetPos;
    RotationState.Speed = InterpSpeed;
    RotationState.bIsActive = true;
}

void UPathMover::RotateActorToward(const FVector& TargetPos, float InterpSpeed)
{
    if (!GetOwner()) return;

    CurrentMovingComponent = nullptr;
    RotationState.TargetPos = TargetPos;
    RotationState.Speed = InterpSpeed;
    RotationState.bIsActive = true;
}

void UPathMover::UpdateRotation(float DeltaTime)
{
    if (!RotationState.bIsActive) return;

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
        RotationState.bIsActive = false;
        return;
    }

    // 목표를 향하는 방향 계산
    FVector DirectionToTarget = (RotationState.TargetPos - CurrentLocation).GetSafeNormal();
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
        
        RotationState.bIsActive = false;
        OnRotationCompleted.Broadcast();
        return;
    }

    // 부드러운 회전 적용
    FRotator NewRotation = FRotator(0.0f, 
        FMath::FInterpTo(CurrentRotation.Yaw, TargetRotation.Yaw, DeltaTime, RotationState.Speed), 
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

float UPathMover::CalculateInterpolationAlpha(float RawAlpha, EMovementInterpolationType InterpType)
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