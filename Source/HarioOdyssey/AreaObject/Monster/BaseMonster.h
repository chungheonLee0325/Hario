﻿/*
BaseMonster System
├── Core Features
│   ├── Monster Data
│   │   ├── Base Stats (TODO: DataTable)
│   │   ├── Current Stats
│   │   └── Combat Stats (TODO: DataTable)
│   ├── Movement System
│   │   ├── Movement
│   │   ├── Rotation
│   │   └── Component Movement
│   └── Combat System
│       ├── Skill Management
│       ├── Target Management
│       └── State Management
├── AI Features
│   ├── State Machine
│   │   ├── Idle
│   │   ├── Chase
│   │   ├── Attack
│   │   └── BackHome
│   └── Aggro System
│       ├── Target Selection
│       ├── Range Check
│       └── Aggro Table
└── Utility Features
    ├── Movement Interface
    │   ├── Actor Movement
    │   ├── Component Movement
    │   └── Rotation Control
    └── Combat Interface
        ├── Skill Usage
        ├── Target Selection
        └── State Changes
*/

#pragma once

#include "CoreMinimal.h"
#include "HarioOdyssey/AreaObject/Base/AreaObject.h"
#include "HarioOdyssey/PathMover/PathMover.h"
#include "BaseMonster.generated.h"

class UBaseSkill;
class UPathMover;

USTRUCT(BlueprintType)
struct FMonsterData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta=(Comment="TODO: Move to DataTable"))
	float MaxHealth = 1.0f;

	UPROPERTY(EditAnywhere, meta=(Comment="TODO: Move to DataTable"))
	float DetectionRange = 1000.0f;

	UPROPERTY(EditAnywhere, meta=(Comment="TODO: Move to DataTable"))
	float MovementSpeed = 300.0f;

	UPROPERTY(EditAnywhere, meta=(Comment="TODO: Move to DataTable"))
	float RotationSpeed = 360.0f;
};

UCLASS()
class HARIOODYSSEY_API ABaseMonster : public AAreaObject
{
	GENERATED_BODY()

public:
	ABaseMonster();

	// Core Functions
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	virtual void OnBodyBeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult & SweepResult);
	

	// Movement Interface
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveToLocation(const FVector& Target, float Duration, 
					   EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveToLocationWithSpeed(const FVector& Target, float Speed,
								 EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveToActor(AActor* Target, float Duration, 
					EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveToActorWithSpeed(const AActor* Target, float Speed,
							 EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);

	// Component Movement Interface
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveComponentToLocation(USceneComponent* ComponentToMove, const FVector& Target, 
							   float Duration, EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveComponentToLocationWithSpeed(USceneComponent* ComponentToMove, const FVector& Target,
										float Speed, EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ReturnComponentToOriginal(USceneComponent* ComponentToReturn, float Duration, 
								 EMovementInterpolationType InterpType = EMovementInterpolationType::Linear, 
								 bool bStickToGround = false);
	
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void ReturnComponentToOriginalWithSpeed(USceneComponent* ComponentToReturn, float Speed,
	EMovementInterpolationType InterpType, bool bStickToGround);
	
	// Rotation Interface
	UFUNCTION(BlueprintCallable, Category = "Rotation")
	void RotateToDirection(const FRotator& Target, float Duration,
						  EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);

	UFUNCTION(BlueprintCallable, Category = "Rotation")
	void RotateToDirectionWithSpeed(const FRotator& Target, float Speed,
								  EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);

	UFUNCTION(BlueprintCallable, Category = "Rotation")
	void LookAtLocation(const FVector& Target, float Duration,
						EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);

	UFUNCTION(BlueprintCallable, Category = "Rotation")
	void LookAtLocationWithSpeed(const FVector& Target, float Speed,
								EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);

	// Component Rotation Interface
	UFUNCTION(BlueprintCallable, Category = "Movement")
	void LookAtComponentToLocation(USceneComponent* ComponentToRotate, const FVector& TargetLocation, 
										   float Duration, EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void LookAtComponentToLocationWithSpeed(USceneComponent* ComponentToRotate, const FVector& TargetLocation, 
												float Speed, EMovementInterpolationType InterpType = EMovementInterpolationType::Linear);


	// Combat Interface
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual UBaseSkill* GetCurrentSkill();
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual AActor* GetAggroTarget() const;
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual bool CanCastSkill(UBaseSkill* Skill, const AActor* Target) const;
	
	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void CastSkill(UBaseSkill* Skill, const AActor* Target);

	// State Checks
	UFUNCTION(BlueprintPure, Category = "State")
	bool IsMoving() const;
	
	UFUNCTION(BlueprintPure, Category = "State")
	bool IsRotating() const;

	// Control Interface
	UFUNCTION(BlueprintCallable, Category = "Control")
	void StopMoving();

	UFUNCTION(BlueprintCallable, Category = "Control")
	void StopRotating();

	UFUNCTION(BlueprintCallable, Category = "Control")
	void StopAll();

	// Combat System
	UFUNCTION(BlueprintCallable, Category = "Combat")
	UBaseSkill* GetSkillByState(EAiStateType StateType) const;

	UFUNCTION(BlueprintCallable, Category = "Combat")
	virtual void SetAggroTarget(AActor* NewTarget) { m_AggroTarget = NewTarget; }

	// Data Access
	const FMonsterData& GetMonsterData() const { return MonsterData; }

	// Monster Data
	UPROPERTY(EditAnywhere, Category = "Monster Settings")
	FMonsterData MonsterData;
	
protected:


	// Skill System
	UPROPERTY(EditAnywhere, Category = "Combat")
	TArray<TSubclassOf<UBaseSkill>> SkillClasses;
	
	UPROPERTY()
	TArray<UBaseSkill*> Skills;
		
	UPROPERTY(EditAnywhere, Category = "Combat")
	TMap<EAiStateType, TSubclassOf<UBaseSkill>> StateSkillMap;
	
	UPROPERTY()
	TMap<EAiStateType, UBaseSkill*> m_StateSkillInstances;
	
	// Movement System
	UPROPERTY(BlueprintReadWrite)
	UPathMover* m_PathMover;

	// Event Handlers
	UFUNCTION()
	virtual void OnMovementFinished();
	
	UFUNCTION()
	virtual void OnRotationFinished();

	// Combat System
	UPROPERTY()
	AActor* m_AggroTarget;

	UPROPERTY()
	FVector m_SpawnLocation;

private:
	bool IsValidForMovement() const;

	UPROPERTY()
	AActor* m_CurrentTarget;
};
