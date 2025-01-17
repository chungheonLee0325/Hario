// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HarioOdyssey/AreaObject/Attribute/Condition.h"
#include "HarioOdyssey/ResourceManager/HarioGameType.h"
#include "AreaObject.generated.h"

class UCondition;

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

	// 초기화 로직
	virtual void PostInitializeComponents() override;

UPROPERTY()
	class UHealth* m_Health;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void CalcDamage(float Damage, AActor* Caster, AActor* Target, bool IsPointDamage = false);
	
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	virtual void OnDie();

	virtual void OnKill();

	virtual void OnRevival();

	bool IsDie() const { return m_Condition->IsDead(); }

	bool AddCondition(EConditionType Condition) const;
	bool RemoveCondition(EConditionType Condition) const;
	bool HasCondition(EConditionType Condition) const;
	

	UPROPERTY(Blueprintable)
	int m_AreaObjectID;

private:
	


UPROPERTY()
	UCondition* m_Condition;
	
	// 스마트 포인터 사용?
	struct FAreaObjectData* dt_AreaObject;
	
};
