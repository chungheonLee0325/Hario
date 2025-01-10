// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HatProjectile.generated.h"

class APlayer_Mario;

UCLASS()
class HARIOODYSSEY_API AHatProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHatProjectile();
	
	void SetReturning(bool bReturning);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitHatProjectile(APlayer_Mario* OwnerCharacter_);
private:
	// bool bIsHatThrown = false;
 //    AActor* CurrentHat = nullptr;
 //
 //    UPROPERTY(EditDefaultsOnly, Category = "Hat")
 //    TSubclassOf<AActor> HatProjectileClass;
 //
 //    UPROPERTY(EditDefaultsOnly, Category = "Hat")
 //    float HatThrowForce = 1000.0f;

	
	bool bIsReturning = false;

	UPROPERTY(EditDefaultsOnly, Category = "Hat")
	float ReturnSpeed = 500.0f;

	APlayer_Mario* OwnerCharacter=nullptr;
};
