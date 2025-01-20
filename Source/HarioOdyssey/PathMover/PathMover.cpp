// Fill out your copyright notice in the Description page of Project Settings.


#include "PathMover.h"
#include "DrawDebugHelpers.h"

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

	const float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastUpdateTime < MinUpdateInterval)
	{
		return;
	}
	LastUpdateTime = CurrentTime;

	// Update Movement
	if (MovementState.IsActive())
	{
		MovementState.Update(DeltaTime);
		const float Alpha = MovementState.GetProgress();
		const float InterpolatedAlpha = CalculateInterpolationAlpha(Alpha, MovementState.InterpType);

		// 실제 이동 적용
		const FVector NewPosition = FMath::Lerp(MovementState.StartPos, MovementState.TargetPos, InterpolatedAlpha);
		if (CurrentMovingComponent.IsValid())
		{
			CurrentMovingComponent->SetWorldLocation(NewPosition);
		}
		else if (AActor* Owner = GetOwner())
		{
			Owner->SetActorLocation(NewPosition);
		}

		OnMovementProgress.Broadcast(Alpha);

		if (Alpha >= 1.0f)
		{
			MovementState.Stop();
			OnMovementCompleted.Broadcast();
		}
	}

	// Update Rotation
	if (RotationState.IsActive())
	{
		RotationState.Update(DeltaTime);
		const float Alpha = RotationState.GetProgress();
		const float InterpolatedAlpha = CalculateInterpolationAlpha(Alpha, RotationState.InterpType);

		// 실제 회전 적용
		const FRotator NewRotation = FMath::Lerp(RotationState.StartRotation, RotationState.TargetRotation,
		                                         InterpolatedAlpha);
		if (CurrentRotatingComponent.IsValid())
		{
			CurrentRotatingComponent->SetWorldRotation(NewRotation);
		}
		else if (AActor* Owner = GetOwner())
		{
			Owner->SetActorRotation(NewRotation);
		}

		OnRotationProgress.Broadcast(Alpha);

		if (Alpha >= 1.0f)
		{
			RotationState.Stop();
			OnRotationCompleted.Broadcast();
		}
	}

	// Debug Visualization
	if (bShowDebugPath || bShowRotationDebug)
	{
		DrawDebugVisuals();
	}
}

// Movement Interface Implementation
void UPathMover::MoveComponentTo(USceneComponent* ComponentToMove, const FVector& TargetPosition,
                                 float Duration, EMovementInterpolationType InterpType, bool bStickToGround)
{
	if (!ComponentToMove || Duration <= 0.0f) return;

	FVector AdjustedTargetPosition = TargetPosition;

	if (bStickToGround)
	{
		FHitResult HitResult;
		FVector TraceStart = TargetPosition + FVector(0.0f, 0.0f, 1000.0f);
		FVector TraceEnd = TargetPosition - FVector(0.0f, 0.0f, 1000.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetOwner());

		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			AdjustedTargetPosition.Z = HitResult.Location.Z;
		}
	}

	StartNewMovement(ComponentToMove, AdjustedTargetPosition, EPMMovementMode::Duration, Duration, InterpType);
}

void UPathMover::MoveComponentToWithSpeed(USceneComponent* ComponentToMove, const FVector& TargetPosition,
                                          float Speed, EMovementInterpolationType InterpType, bool bStickToGround)
{
	if (!ComponentToMove || Speed <= 0.0f) return;

	FVector AdjustedTargetPosition = TargetPosition;

	if (bStickToGround)
	{
		FHitResult HitResult;
		FVector TraceStart = TargetPosition + FVector(0.0f, 0.0f, 1000.0f);
		FVector TraceEnd = TargetPosition - FVector(0.0f, 0.0f, 1000.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetOwner());

		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			AdjustedTargetPosition.Z = HitResult.Location.Z;
		}
	}

	StartNewMovement(ComponentToMove, AdjustedTargetPosition, EPMMovementMode::Speed, Speed, InterpType);
}

void UPathMover::MoveActorTo(const FVector& TargetPosition, float Duration,
                             EMovementInterpolationType InterpType, bool bStickToGround)
{
	if (!GetOwner()) return;

	FVector AdjustedTargetPosition = TargetPosition;

	if (bStickToGround)
	{
		// 라인 트레이스를 통해 지형의 높이를 감지
		FHitResult HitResult;
		FVector TraceStart = TargetPosition + FVector(0.0f, 0.0f, 1000.0f); // 위에서 시작
		FVector TraceEnd = TargetPosition - FVector(0.0f, 0.0f, 1000.0f); // 아래로 트레이스

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetOwner());

		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			AdjustedTargetPosition.Z = HitResult.Location.Z;
		}
	}

	StartNewMovement(nullptr, AdjustedTargetPosition, EPMMovementMode::Duration, Duration, InterpType);
}

void UPathMover::MoveActorToWithSpeed(const FVector& TargetPosition, float Speed,
                                      EMovementInterpolationType InterpType, bool bStickToGround)
{
	if (!GetOwner()) return;

	FVector AdjustedTargetPosition = TargetPosition;

	if (bStickToGround)
	{
		// 라인 트레이스를 통해 지형의 높이를 감지
		FHitResult HitResult;
		FVector TraceStart = TargetPosition + FVector(0.0f, 0.0f, 1000.0f); // 위에서 시작
		FVector TraceEnd = TargetPosition - FVector(0.0f, 0.0f, 1000.0f); // 아래로 트레이스

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetOwner());

		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			AdjustedTargetPosition.Z = HitResult.Location.Z;
		}
	}

	StartNewMovement(nullptr, AdjustedTargetPosition, EPMMovementMode::Speed, Speed, InterpType);
}

void UPathMover::ReturnComponentToOriginal(USceneComponent* ComponentToReturn, float Duration,
                                           EMovementInterpolationType InterpType, bool bStickToGround)
{
	if (!ComponentToReturn) return;

	const FVector* OriginalPos = OriginalPositions.Find(ComponentToReturn);
	if (!OriginalPos) return;

	FVector AdjustedOriginalPosition = *OriginalPos;

	if (bStickToGround)
	{
		FHitResult HitResult;
		FVector TraceStart = AdjustedOriginalPosition + FVector(0.0f, 0.0f, 1000.0f);
		FVector TraceEnd = AdjustedOriginalPosition - FVector(0.0f, 0.0f, 1000.0f);

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(GetOwner());

		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
		{
			AdjustedOriginalPosition.Z = HitResult.Location.Z;
		}
	}

	if (USceneComponent* Parent = ComponentToReturn->GetAttachParent())
	{
		const FVector WorldPos = Parent->GetComponentTransform().TransformPosition(AdjustedOriginalPosition);
		MoveComponentTo(ComponentToReturn, WorldPos, Duration, InterpType, bStickToGround);
	}
	else
	{
		MoveComponentTo(ComponentToReturn, AdjustedOriginalPosition, Duration, InterpType, bStickToGround);
	}
}

// Rotation Interface Implementation
void UPathMover::RotateComponentTo(USceneComponent* ComponentToRotate, const FRotator& TargetRotation,
                                   float Duration, EMovementInterpolationType InterpType)
{
	if (!ComponentToRotate || Duration <= 0.0f) return;
	StartNewRotation(ComponentToRotate, TargetRotation, EPMRotationMode::Duration, Duration, InterpType);
}

void UPathMover::RotateComponentToWithSpeed(USceneComponent* ComponentToRotate, const FRotator& TargetRotation,
                                            float Speed, EMovementInterpolationType InterpType)
{
	if (!ComponentToRotate || Speed <= 0.0f) return;
	StartNewRotation(ComponentToRotate, TargetRotation, EPMRotationMode::Speed, Speed, InterpType);
}

void UPathMover::RotateActorTo(const FRotator& TargetRotation, float Duration,
                               EMovementInterpolationType InterpType)
{
	if (!GetOwner() || Duration <= 0.0f) return;
	StartNewRotation(nullptr, TargetRotation, EPMRotationMode::Duration, Duration, InterpType);
}

void UPathMover::RotateActorToWithSpeed(const FRotator& TargetRotation, float Speed,
                                        EMovementInterpolationType InterpType)
{
	if (!GetOwner() || Speed <= 0.0f) return;
	StartNewRotation(nullptr, TargetRotation, EPMRotationMode::Speed, Speed, InterpType);
}

void UPathMover::LookAtLocationWithComponent(USceneComponent* ComponentToRotate, const FVector& TargetLocation,
                                             float Duration, EMovementInterpolationType InterpType)
{
	if (!ComponentToRotate || Duration <= 0.0f) return;

	const FVector ComponentLocation = ComponentToRotate->GetComponentLocation();
	const FVector Direction = (TargetLocation - ComponentLocation).GetSafeNormal();
	const FRotator TargetRotation = Direction.Rotation();

	RotateComponentTo(ComponentToRotate, TargetRotation, Duration, InterpType);
}

void UPathMover::LookAtLocationWithComponentSpeed(USceneComponent* ComponentToRotate, const FVector& TargetLocation,
                                                  float Speed, EMovementInterpolationType InterpType)
{
	if (!ComponentToRotate || Speed <= 0.0f) return;

	const FVector ComponentLocation = ComponentToRotate->GetComponentLocation();
	const FVector Direction = (TargetLocation - ComponentLocation).GetSafeNormal();
	const FRotator TargetRotation = Direction.Rotation();

	RotateComponentToWithSpeed(ComponentToRotate, TargetRotation, Speed, InterpType);
}

void UPathMover::LookAtLocationWithActor(const FVector& TargetLocation, float Duration,
                                         EMovementInterpolationType InterpType)
{
	if (!GetOwner() || Duration <= 0.0f) return;

	const FVector ActorLocation = GetOwner()->GetActorLocation();
	const FVector Direction = (TargetLocation - ActorLocation).GetSafeNormal();
	const FRotator TargetRotation = Direction.Rotation();

	RotateActorTo(TargetRotation, Duration, InterpType);
}

void UPathMover::LookAtLocationWithActorSpeed(const FVector& TargetLocation, float Speed,
                                              EMovementInterpolationType InterpType)
{
	if (!GetOwner() || Speed <= 0.0f) return;

	const FVector ActorLocation = GetOwner()->GetActorLocation();
	const FVector Direction = (TargetLocation - ActorLocation).GetSafeNormal();
	const FRotator TargetRotation = Direction.Rotation();

	RotateActorToWithSpeed(TargetRotation, Speed, InterpType);
}

// State Control Implementation
void UPathMover::StopMovement()
{
	if (!MovementState.IsActive()) return;

	if (CurrentMovingComponent.IsValid())
	{
		const FVector CurrentPos = CurrentMovingComponent->GetComponentLocation();
		CurrentMovingComponent->SetWorldLocation(CurrentPos);
	}
	else if (AActor* Owner = GetOwner())
	{
		const FVector CurrentPos = Owner->GetActorLocation();
		Owner->SetActorLocation(CurrentPos);
	}

	MovementState = FMovementState();
	ResetComponentReference();
	OnMovementCompleted.Broadcast();
}

void UPathMover::StopRotation()
{
	if (!RotationState.IsActive()) return;

	if (CurrentRotatingComponent.IsValid())
	{
		const FRotator CurrentRot = CurrentRotatingComponent->GetComponentRotation();
		CurrentRotatingComponent->SetWorldRotation(CurrentRot);
	}
	else if (AActor* Owner = GetOwner())
	{
		const FRotator CurrentRot = Owner->GetActorRotation();
		Owner->SetActorRotation(CurrentRot);
	}

	RotationState = FRotationState();
	ResetComponentReference();
	OnRotationCompleted.Broadcast();
}

// Internal Implementation
void UPathMover::StartNewMovement(USceneComponent* ComponentToMove, const FVector& Target,
                                  EPMMovementMode Mode, float SpeedOrDuration,
                                  EMovementInterpolationType InterpType)
{
	StopMovement();

	CurrentMovingComponent = ComponentToMove;
	MovementState.StartPos = ComponentToMove ? ComponentToMove->GetComponentLocation() : GetOwner()->GetActorLocation();
	MovementState.TargetPos = Target;
	MovementState.MovementMode = Mode;
	MovementState.InterpType = InterpType;

	if (Mode == EPMMovementMode::Duration)
	{
		MovementState.Duration = SpeedOrDuration;
		MovementState.Speed = 0.0f;
	}
	else
	{
		MovementState.Speed = SpeedOrDuration;
		MovementState.Duration = (Target - MovementState.StartPos).Size() / SpeedOrDuration;
	}

	MovementState.CurrentTime = 0.0f;
	MovementState.bIsActive = true;
}

void UPathMover::StartNewRotation(USceneComponent* ComponentToRotate, const FRotator& TargetRot,
                                  EPMRotationMode Mode, float SpeedOrDuration,
                                  EMovementInterpolationType InterpType)
{
	StopRotation();

	CurrentRotatingComponent = ComponentToRotate;
	RotationState.StartRotation = ComponentToRotate
		                              ? ComponentToRotate->GetComponentRotation()
		                              : GetOwner()->GetActorRotation();
	RotationState.TargetRotation = TargetRot;
	RotationState.RotationMode = Mode;
	RotationState.InterpType = InterpType;

	if (Mode == EPMRotationMode::Duration)
	{
		RotationState.Duration = SpeedOrDuration;
		RotationState.Speed = 0.0f;
	}
	else
	{
		RotationState.Speed = SpeedOrDuration;
		const float MaxRotationDelta = FMath::Max(
			FMath::Abs(FMath::FindDeltaAngleDegrees(RotationState.StartRotation.Yaw, TargetRot.Yaw)),
			FMath::Max(
				FMath::Abs(FMath::FindDeltaAngleDegrees(RotationState.StartRotation.Pitch, TargetRot.Pitch)),
				FMath::Abs(FMath::FindDeltaAngleDegrees(RotationState.StartRotation.Roll, TargetRot.Roll))
			)
		);
		RotationState.Duration = MaxRotationDelta / SpeedOrDuration;
	}

	RotationState.CurrentTime = 0.0f;
	RotationState.bIsActive = true;
}

void UPathMover::ResetComponentReference()
{
	if (!MovementState.IsActive())
	{
		CurrentMovingComponent.Reset();
	}
	if (!RotationState.IsActive())
	{
		CurrentRotatingComponent.Reset();
	}
}

float UPathMover::CalculateInterpolationAlpha(float RawAlpha, EMovementInterpolationType InterpType)
{
	const float n1 = 7.5625;
	const float d1 = 2.75;
	const float c4 = (2 * PI) / 3;
	switch (InterpType)
	{
	case EMovementInterpolationType::Linear:
		return RawAlpha;
	case EMovementInterpolationType::EaseIn:
		return RawAlpha * RawAlpha;
	case EMovementInterpolationType::EaseOut:
		return 1.0f - FMath::Square(1.0f - RawAlpha);
	case EMovementInterpolationType::EaseInOut:
		return RawAlpha < 0.5f ? 2.0f * RawAlpha * RawAlpha : 1.0f - FMath::Pow(-2.0f * RawAlpha + 2.0f, 2.0f) / 2.0f;
	case EMovementInterpolationType::EaseOutBounce:
		if (RawAlpha < 1 / d1)
		{
			return n1 * RawAlpha * RawAlpha;
		}
		else if (RawAlpha < 2 / d1)
		{
			return n1 * (RawAlpha -= 1.5 / d1) * RawAlpha + 0.75;
		}
		else if (RawAlpha < 2.5 / d1)
		{
			return n1 * (RawAlpha -= 2.25 / d1) * RawAlpha + 0.9375;
		}
		else
		{
			return n1 * (RawAlpha -= 2.625 / d1) * RawAlpha + 0.984375;
		}
	case EMovementInterpolationType::EaseOutElastic:
		return RawAlpha == 0
		  ? 0
		  : RawAlpha == 1
		  ? 1
		  : FMath::Pow(2, -10 * RawAlpha) * FMath::Sin((RawAlpha * 10 - 0.75) * c4) + 1;
	default:
		return RawAlpha;
	}
}

void UPathMover::DrawDebugVisuals() const
{
	if (!GetWorld()) return;

	if (bShowDebugPath && MovementState.IsActive())
	{
		DrawDebugLine(GetWorld(), MovementState.StartPos, MovementState.TargetPos,
		              FColor::Green, false, -1.0f, 0, 2.0f);
		DrawDebugPoint(GetWorld(), MovementState.TargetPos, 10.0f, FColor::Red, false, -1.0f);
	}

	if (bShowRotationDebug && RotationState.IsActive())
	{
		const FVector CurrentLocation = CurrentRotatingComponent.IsValid()
			                                ? CurrentRotatingComponent->GetComponentLocation()
			                                : GetOwner()->GetActorLocation();

		DrawDebugLine(GetWorld(), CurrentLocation, RotationState.TargetRotation.Vector() * 100.0f + CurrentLocation,
		              FColor::Blue, false, -1.0f, 0, 2.0f);
	}
}

// State Class Implementations
void FMovementState::Update(float DeltaTime)
{
	if (!bIsActive) return;

	CurrentTime += DeltaTime;
	if (MovementMode == EPMMovementMode::Speed)
	{
		Duration = (TargetPos - StartPos).Size() / Speed;
	}
}

void FMovementState::Stop()
{
	bIsActive = false;
	CurrentTime = 0.0f;
	Duration = 0.0f;
	Speed = 0.0f;
	StartPos = FVector::ZeroVector;
	TargetPos = FVector::ZeroVector;
	MovementMode = EPMMovementMode::Duration;
	InterpType = EMovementInterpolationType::Linear;
}

void FRotationState::Update(float DeltaTime)
{
	if (!bIsActive) return;

	CurrentTime += DeltaTime;
	if (RotationMode == EPMRotationMode::Speed)
	{
		const float MaxRotationDelta = FMath::Max(
			FMath::Abs(FMath::FindDeltaAngleDegrees(StartRotation.Yaw, TargetRotation.Yaw)),
			FMath::Max(
				FMath::Abs(FMath::FindDeltaAngleDegrees(StartRotation.Pitch, TargetRotation.Pitch)),
				FMath::Abs(FMath::FindDeltaAngleDegrees(StartRotation.Roll, TargetRotation.Roll))
			)
		);
		Duration = MaxRotationDelta / Speed;
	}
}

void FRotationState::Stop()
{
	bIsActive = false;
	CurrentTime = 0.0f;
	Duration = 0.0f;
	Speed = 0.0f;
	StartRotation = FRotator::ZeroRotator;
	TargetRotation = FRotator::ZeroRotator;
	RotationMode = EPMRotationMode::Duration;
	InterpType = EMovementInterpolationType::Linear;
}
