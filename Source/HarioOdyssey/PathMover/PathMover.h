// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PathMover.generated.h"

// Forward declarations

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMovementCompleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRotationCompleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMovementProgress, float, Progress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRotationProgress, float, Progress);

// Enums
UENUM(BlueprintType)
enum class EMovementInterpolationType : uint8
{
    Linear     UMETA(DisplayName = "Linear"),
    EaseIn     UMETA(DisplayName = "Ease In"),
    EaseOut    UMETA(DisplayName = "Ease Out"),
    EaseInOut  UMETA(DisplayName = "Ease In Out")
};

UENUM(BlueprintType)
enum class EPMMovementMode : uint8
{
    Duration    UMETA(DisplayName = "Duration Based"),
    Speed       UMETA(DisplayName = "Speed Based")
};

UENUM(BlueprintType)
enum class EPMRotationMode : uint8
{
    Duration    UMETA(DisplayName = "Duration Based"),
    Speed       UMETA(DisplayName = "Speed Based")
};

// State Classes
class FPathMoverState
{
public:
    virtual ~FPathMoverState() = default;
    virtual void Update(float DeltaTime) = 0;
    virtual void Stop() = 0;
    virtual bool IsActive() const = 0;
    virtual float GetProgress() const = 0;
};

class FMovementState : public FPathMoverState
{
public:
    virtual void Update(float DeltaTime) override;
    virtual void Stop() override;
    virtual bool IsActive() const override { return bIsActive; }
    virtual float GetProgress() const override { return CurrentTime / Duration; }

    bool bIsActive = false;
    float CurrentTime = 0.0f;
    float Duration = 0.0f;
    float Speed = 0.0f;
    FVector StartPos = FVector::ZeroVector;
    FVector TargetPos = FVector::ZeroVector;
    EPMMovementMode MovementMode = EPMMovementMode::Duration;
    EMovementInterpolationType InterpType = EMovementInterpolationType::Linear;
};

class FRotationState : public FPathMoverState
{
public:
    virtual void Update(float DeltaTime) override;
    virtual void Stop() override;
    virtual bool IsActive() const override { return bIsActive; }
    virtual float GetProgress() const override { return CurrentTime / Duration; }

    bool bIsActive = false;
    float CurrentTime = 0.0f;
    float Duration = 0.0f;
    float Speed = 5.0f;
    FRotator StartRotation = FRotator::ZeroRotator;
    FRotator TargetRotation = FRotator::ZeroRotator;
    EPMRotationMode RotationMode = EPMRotationMode::Speed;
    EMovementInterpolationType InterpType = EMovementInterpolationType::Linear;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HARIOODYSSEY_API UPathMover : public UActorComponent
{
    GENERATED_BODY()

public:    
    UPathMover();

    // Movement Interface
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveComponentTo(USceneComponent* ComponentToMove, const FVector& TargetPosition, 
                        float Duration, EMovementInterpolationType InterpType = EMovementInterpolationType::Linear, 
                        bool bStickToGround = false);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveComponentToWithSpeed(USceneComponent* ComponentToMove, const FVector& TargetPosition, 
                                float Speed, EMovementInterpolationType InterpType = EMovementInterpolationType::Linear, 
                                bool bStickToGround = false);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveActorTo(const FVector& TargetPosition, float Duration, 
                     EMovementInterpolationType InterpType = EMovementInterpolationType::Linear, 
                     bool bStickToGround = false);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveActorToWithSpeed(const FVector& TargetPosition, float Speed, 
                             EMovementInterpolationType InterpType = EMovementInterpolationType::Linear, 
                             bool bStickToGround = false);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void ReturnComponentToOriginal(USceneComponent* ComponentToReturn, float Duration, 
                                 EMovementInterpolationType InterpType = EMovementInterpolationType::Linear, 
                                 bool bStickToGround = false);

    // Rotation Interface
    UFUNCTION(BlueprintCallable, Category = "Rotation")
    void RotateComponentTo(USceneComponent* ComponentToRotate, const FRotator& TargetRotation, 
                          float Duration, EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);

    UFUNCTION(BlueprintCallable, Category = "Rotation")
    void RotateComponentToWithSpeed(USceneComponent* ComponentToRotate, const FRotator& TargetRotation, 
                                  float Speed, EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);

    UFUNCTION(BlueprintCallable, Category = "Rotation")
    void RotateActorTo(const FRotator& TargetRotation, float Duration, 
                       EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);

    UFUNCTION(BlueprintCallable, Category = "Rotation")
    void RotateActorToWithSpeed(const FRotator& TargetRotation, float Speed, 
                               EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);

    // Look At Interface
    UFUNCTION(BlueprintCallable, Category = "Rotation")
    void LookAtLocationWithComponent(USceneComponent* ComponentToRotate, const FVector& TargetLocation, 
                                   float Duration, EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);

    UFUNCTION(BlueprintCallable, Category = "Rotation")
    void LookAtLocationWithComponentSpeed(USceneComponent* ComponentToRotate, const FVector& TargetLocation, 
                                        float Speed, EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);

    UFUNCTION(BlueprintCallable, Category = "Rotation")
    void LookAtLocationWithActor(const FVector& TargetLocation, float Duration, 
                                EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);

    UFUNCTION(BlueprintCallable, Category = "Rotation")
    void LookAtLocationWithActorSpeed(const FVector& TargetLocation, float Speed, 
                                     EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);

    // Control Interface
    UFUNCTION(BlueprintCallable, Category = "Control")
    void StopMovement();

    UFUNCTION(BlueprintCallable, Category = "Control")
    void StopRotation();

    UFUNCTION(BlueprintCallable, Category = "Control")
    void StopAll() { StopMovement(); StopRotation(); }

    // State Queries
    UFUNCTION(BlueprintCallable, Category = "State")
    bool IsMoving() const { return MovementState.IsActive(); }

    UFUNCTION(BlueprintCallable, Category = "State")
    bool IsRotating() const { return RotationState.IsActive(); }

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMovementCompleted OnMovementCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnRotationCompleted OnRotationCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMovementProgress OnMovementProgress;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnRotationProgress OnRotationProgress;

    // Configuration
    UPROPERTY(EditAnywhere, Category = "Performance")
    float MinUpdateInterval = 0.0f;

    UPROPERTY(EditAnywhere, Category = "Performance")
    float LocationUpdateThreshold = 0.1f;

    UPROPERTY(EditAnywhere, Category = "Performance")
    float RotationUpdateThreshold = 0.1f;

    UPROPERTY(EditAnywhere, Category = "Debug")
    bool bShowDebugPath = false;

    UPROPERTY(EditAnywhere, Category = "Debug")
    bool bShowRotationDebug = false;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
                             FActorComponentTickFunction* ThisTickFunction) override;

private:
    // State
    FMovementState MovementState;
    FRotationState RotationState;

    // Component References
    TWeakObjectPtr<USceneComponent> CurrentMovingComponent;
    TWeakObjectPtr<USceneComponent> CurrentRotatingComponent;
    TMap<TWeakObjectPtr<USceneComponent>, FVector> OriginalPositions;

    // Timing
    float LastUpdateTime = 0.0f;

    // Internal Methods
    void StartNewMovement(USceneComponent* ComponentToMove, const FVector& Target, 
                         EPMMovementMode Mode, float SpeedOrDuration, 
                         EMovementInterpolationType InterpType);
    void StartNewRotation(USceneComponent* ComponentToRotate, const FRotator& TargetRot, 
                         EPMRotationMode Mode, float SpeedOrDuration, 
                         EMovementInterpolationType InterpType);
    void ResetComponentReference();
    void DrawDebugVisuals() const;
    static float CalculateInterpolationAlpha(float RawAlpha, EMovementInterpolationType InterpType);
}; 