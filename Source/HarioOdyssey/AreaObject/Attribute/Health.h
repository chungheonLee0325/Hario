﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Health.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class HARIOODYSSEY_API UHealth : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UHealth();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	//Health 초기화
	void InitHealth(float hpMax);

	// 체력 증감
	UFUNCTION()
	float IncreaseHP(float Delta);

	UFUNCTION()
	void SetHPByRate(float Rate);

	UFUNCTION()
	float GetHP() const;
	


private:
	
UPROPERTY(VisibleAnywhere)
	float m_HPMax = 1.0f;
	
UPROPERTY(VisibleAnywhere)
	float m_HP;
};
