// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VerticalMover.generated.h"

UENUM(BlueprintType)
enum class EMoveState : uint8
{
	Rising,
	Falling,
	Stopped
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HARIOODYSSEY_API UVerticalMover : public UActorComponent
{
	GENERATED_BODY()

public:    
	UVerticalMover();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StartVerticalMovement(USceneComponent* Target, float TargetHeight, float RiseTime, float FallTime);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StopVerticalMovement();

	UFUNCTION(BlueprintPure, Category = "Movement")
	EMoveState GetCurrentMoveState() const { return CurrentMoveState; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bAutoReturnToStart = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float HoverTime = 0.0f;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	USceneComponent* TargetComponent;
    
	FVector InitialRelativeLocation;
	float TargetHeight;
	float RiseTime;
	float FallTime;
	float CurrentTime;
	float HoverTimer;
	bool bIsMoving;
    
	EMoveState CurrentMoveState;
    
	float CalculateVerticalOffset(float CurrentTime) const;
	void UpdateMovement(float DeltaTime);
};