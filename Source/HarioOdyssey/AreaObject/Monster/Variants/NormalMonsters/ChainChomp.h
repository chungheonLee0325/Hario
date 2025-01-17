// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HarioOdyssey/AreaObject/Monster/BaseMonster.h"
#include "HarioOdyssey/Combat/Capturable.h"
#include "ChainChomp.generated.h"

class UAiChainChomp;
class UPathMover;
class USphereComponent;

UCLASS()
class HARIOODYSSEY_API AChainChomp : public ABaseMonster, public ICapturable
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AChainChomp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* ChainChompRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* ChainChompMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USphereComponent* ChainChompSphere;

	UPROPERTY(EditAnywhere)
	UPathMover* ChainChompPathMover;

	virtual bool CanBeCaptured_Implementation() override;
	virtual void OnCaptureStart_Implementation() override;
	virtual void OnCaptureEnd_Implementation() override;
	virtual void WhileCaptured_Implementation(ACharacter* CaptureOwner) override;

	// 몬스터의 기본 속성들
	UPROPERTY(EditAnywhere, Category = "Monster Settings")
	float AttackSpeed = 800.0f;
	
	UPROPERTY(EditAnywhere, Category = "Monster Settings")
	float ReturnSpeed = 300.0f;
	
	UPROPERTY(EditAnywhere, Category = "Monster Settings")
	float ChainLength = 3500.0f;

	// Getter 함수들
	float GetAttackSpeed() const { return AttackSpeed; }
	float GetReturnSpeed() const { return ReturnSpeed; }
	float GetChainLength() const { return ChainLength; }


	// 몬스터로 옮겨야할듯
	UPROPERTY()
	UAiChainChomp * m_AiFSM;
};
