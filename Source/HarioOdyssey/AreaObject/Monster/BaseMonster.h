// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HarioOdyssey/AreaObject/Base/AreaObject.h"
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
	//UFUNCTION(BlueprintCallable, Category = "Rotation")
	//void LookAtLocation(const FVector& Target, float Speed);
	//UFUNCTION(BlueprintCallable, Category = "Rotation")
	//void LookAtActor(AActor* Target, float Speed);
	//
	//// Component Rotation Methods
	//UFUNCTION(BlueprintCallable, Category = "Rotation")
	//void ComponentLookAtLocation(USceneComponent* Component, const FVector& Target, float Speed);
	//UFUNCTION(BlueprintCallable, Category = "Rotation")
	//void ComponentLookAtActor(USceneComponent* Component, AActor* Target, float Speed);
	//
	//// Movement Methods
	//UFUNCTION(BlueprintCallable, Category = "Movement")
	//void StopMoving();

	
	
	

UPROPERTY(EditAnywhere,BlueprintReadWrite)
	class UPathMover* m_PathMover;


private:
	TMap<EAiStateType, UBaseSkill*> m_SkillByState;
};
