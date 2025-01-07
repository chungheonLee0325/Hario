// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HarioOdyssey/AreaObject/Attribute/Health.h"
#include "HarioOdyssey/ResourceManager/Hario_Struct.h"
#include "AreaObject.generated.h"

UCLASS()
class HARIOODYSSEY_API AAreaObject : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAreaObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void OnTakeDamaged(float DamageAmount);

	UHealth* m_Health;
	virtual void OnSkill();	// Param은 추후 생각...

	virtual void TakeDamage(float DamageAmount) ;

	

UPROPERTY()
	UHealth* m_Health;	// 가비지 컬렉팅 방지!!

UPROPERTY()
	FAreaObjectData m_AreaObjectData;

private:
	bool m_IsDead = false;
	
};
