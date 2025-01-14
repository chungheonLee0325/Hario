// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PathMover.generated.h"

class UCurveFloat;

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

    // Rotation Interface
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void RotateComponentToward(USceneComponent* ComponentToRotate, const FVector& TargetPos, float InterpSpeed);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void RotateActorToward(const FVector& TargetPos, float InterpSpeed);

    // Return to Original Position
    UFUNCTION(BlueprintCallable, Category = "Movement")
    void ReturnComponentToOriginal(USceneComponent* ComponentToReturn, float Duration, 
                                 EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Movement")
    FOnMovementCompleted OnMovementCompleted;

    UPROPERTY(BlueprintAssignable, Category = "Movement")
    FOnRotationCompleted OnRotationCompleted;

protected:
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
                             FActorComponentTickFunction* ThisTickFunction) override;

private:
    // Movement State
    bool bIsMoving;
    float CurrentLerpTime;
    float MovementDuration;
    FVector StartPosition;
    FVector TargetPosition;
    EMovementInterpolationType CurrentInterpType;
    TWeakObjectPtr<USceneComponent> CurrentMovingComponent;

    // Rotation State
    bool bIsRotating;
    FVector RotationTarget;
    float RotationSpeed;

    // Original Positions Storage
    TMap<TWeakObjectPtr<USceneComponent>, FVector> OriginalPositions;

    // Internal Functions
    void UpdateMovement(float DeltaTime);
    void UpdateRotation(float DeltaTime);
    float CalculateInterpolationAlpha(float RawAlpha, EMovementInterpolationType InterpType) const;
    void StartNewMovement(USceneComponent* ComponentToMove, const FVector& Target, 
                         float Duration, EMovementInterpolationType InterpType);
}; 