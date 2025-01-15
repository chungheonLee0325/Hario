// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HarioOdyssey/AreaObject/Base/AreaObject.h"
#include "HarioOdyssey/PathMover/PathMover.h"
#include "BaseMonster.generated.h"

class UBaseSkill;

UCLASS()
class HARIOODYSSEY_API ABaseMonster : public AAreaObject
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseMonster();
 
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Skill
	virtual UBaseSkill* GetSkillByAiState(EAiStateType StateType);

	// Rotation Methods
	UFUNCTION(BlueprintCallable, Category = "Rotation")
	void StopRotating();
	
	UFUNCTION(BlueprintCallable, Category = "Rotation")
	void LookAtLocation(const FVector& Target, float Speed);

	UFUNCTION(BlueprintCallable, Category = "Rotation")
	void LookAtActor(AActor* Target, float Speed);
	
	// Component Rotation Methods
	UFUNCTION(BlueprintCallable, Category = "Rotation")
	void ComponentLookAtLocation(USceneComponent* Component, const FVector& Target, float Speed);

	UFUNCTION(BlueprintCallable, Category = "Rotation")
	void ComponentLookAtActor(USceneComponent* Component, AActor* Target, float Speed);
	
	// Movement Methods
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void StopMoving();

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveToLocation(const FVector& Target, float Duration, 
					   EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);
    
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveToActor(AActor* Target, float Duration, 
					EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);
    
	// Component Movement
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveComponentToLocation(USceneComponent* ComponentToMove, const FVector& Target, 
							   float Duration, EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveComponentToActor(USceneComponent* ComponentToMove, AActor* Target, float Duration, 
						EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);
	
	// Events
	UFUNCTION()
	virtual void OnMovementFinished();
    
	UFUNCTION()
	virtual void OnRotationFinished();

	// State Checks
	UFUNCTION(BlueprintPure, Category = "State")
	bool IsMoving() const;
    
	UFUNCTION(BlueprintPure, Category = "State")
	bool IsRotating() const;
	
	
	

UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UPathMover* m_PathMover;


private:
	bool IsValidForMovement() const;
	
	TMap<EAiStateType, UBaseSkill*> m_SkillByState;
};
