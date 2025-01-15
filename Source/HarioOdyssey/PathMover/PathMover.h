// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PathMover.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMovementCompleted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnRotationCompleted);

UENUM(BlueprintType)
enum class EMovementInterpolationType : uint8
{
    Linear     UMETA(DisplayName = "Linear"),
    EaseIn     UMETA(DisplayName = "Ease In"),
    EaseOut    UMETA(DisplayName = "Ease Out"),
    EaseInOut  UMETA(DisplayName = "Ease In Out")
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
                        float Duration, EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void MoveActorTo(const FVector& TargetPosition, float Duration, 
                     EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void ReturnComponentToOriginal(USceneComponent* ComponentToReturn, float Duration, 
                                 EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void StopMovement();

    UFUNCTION(BlueprintCallable, Category = "Movement")
    bool IsMoving() const { return MovementState.bIsActive; }

    // Rotation Interface
    UFUNCTION(BlueprintCallable, Category = "Rotation")
    void RotateComponentToward(USceneComponent* ComponentToRotate, const FVector& TargetPos, float InterpSpeed);

    UFUNCTION(BlueprintCallable, Category = "Rotation")
    void RotateActorToward(const FVector& TargetPos, float InterpSpeed);

    UFUNCTION(BlueprintCallable, Category = "Rotation")
    void StopRotation();

    UFUNCTION(BlueprintCallable, Category = "Rotation")
    bool IsRotating() const { return RotationState.bIsActive; }

    // Combined Control
    UFUNCTION(BlueprintCallable, Category = "Control")
    void StopAll() { StopMovement(); StopRotation(); }

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnMovementCompleted OnMovementCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Events")
    FOnRotationCompleted OnRotationCompleted;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
                             FActorComponentTickFunction* ThisTickFunction) override;

private:
    // Movement State
    struct FMovementState
    {
        bool bIsActive = false;
        float CurrentTime = 0.0f;
        float Duration = 0.0f;
        FVector StartPos = FVector::ZeroVector;
        FVector TargetPos = FVector::ZeroVector;
        EMovementInterpolationType InterpType = EMovementInterpolationType::Linear;
    } MovementState;

    // Rotation State
    struct FRotationState
    {
        bool bIsActive = false;
        FVector TargetPos = FVector::ZeroVector;
        float Speed = 5.0f;
    } RotationState;

    // Component Reference
    TWeakObjectPtr<USceneComponent> CurrentMovingComponent;
    TMap<TWeakObjectPtr<USceneComponent>, FVector> OriginalPositions;

    // Internal Functions
    void UpdateMovement(float DeltaTime);
    void UpdateRotation(float DeltaTime);
    static float CalculateInterpolationAlpha(float RawAlpha, EMovementInterpolationType InterpType);
    void StartNewMovement(USceneComponent* ComponentToMove, const FVector& Target, 
                         float Duration, EMovementInterpolationType InterpType);
    void ResetComponentReference();
}; 