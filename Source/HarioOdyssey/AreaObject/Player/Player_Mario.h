// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HarioOdyssey/AreaObject/Base/AreaObject.h"
#include "Player_Mario.generated.h"

class AHatProjectile;

UCLASS()
class HARIOODYSSEY_API APlayer_Mario : public AAreaObject
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	//생성자
	APlayer_Mario();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//입력구성
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//이동 함수
	void OnMoveForward(float Value);
	void OnMoveRight(float Value);
	
	//카메라 회전 함수
	void OnTurn(float Value);
	void OnLookUp(float Value);
	
	//점프입력
	void OnStartJump();
	void OnStopJump();

	//모자던지기,받기
	void OnThrowHat();
	void OnReturnHat();
	
	bool bIsHatThrown = false;
	AHatProjectile* CurrentHat = nullptr;


	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
private:
	//카메라 컴포넌트
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* FollowCamera;

	//모자던지기,받기
	
	UPROPERTY(EditDefaultsOnly, Category = "Hat")
	TSubclassOf<AActor> HatProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category = "Hat")
	float HatThrowForce = 1000.0f;
};
