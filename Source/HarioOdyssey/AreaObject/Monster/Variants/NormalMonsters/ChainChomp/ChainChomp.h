// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HarioOdyssey/AreaObject/Monster/BaseMonster.h"
#include "HarioOdyssey/Capture/Capturable.h"
#include "ChainChomp.generated.h"

class UChainComponent;
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
	virtual UBaseAiFSM* CreateFSM() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnDie() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* ChainChompSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* ChainStartScene;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UChainComponent* ChainComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDestructDmgAble = false;

	UFUNCTION(BlueprintCallable)
	virtual bool CanBeCaptured_Implementation() override;
	UFUNCTION(BlueprintCallable)
	virtual void OnCaptureStart_Implementation() override;
	UFUNCTION(BlueprintCallable)
	virtual void OnCaptureEnd_Implementation() override;
	UFUNCTION(BlueprintCallable)
	virtual void WhileCaptured_Implementation(ACharacter* CaptureOwner) override;
	virtual void OnBodyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	// Getter 함수들
	float GetAttackSpeed() const { return AttackSpeed; }
	float GetReturnSpeed() const { return ReturnSpeed; }
	float GetChainLength() const { return ChainLength; }

	FVector GetRootPosition() const { return RootPosition; };
	FVector GetRootAnchorPosition() const { return RootAnchorPosition; };

private:
	// 몬스터의 기본 속성들
	UPROPERTY(EditAnywhere, Category = "Monster Settings")
	float AttackSpeed = 800.0f;

	UPROPERTY(EditAnywhere, Category = "Monster Settings")
	float ReturnSpeed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Monster Settings")
	float ChainLength = 1200.0f;

	UPROPERTY(EditAnywhere, Category = "Monster Settings")
	FVector RootAnchorPosition = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, Category = "Monster Settings")
	FVector RootPosition;

	UPROPERTY(EditAnywhere, Category = "Monster Settings")
	TArray<UMaterialInterface*> GhostTrailMaterials;

public:
	TArray<UMaterialInterface*> GetGhostTrailMaterials() const;

private:
	UPROPERTY()
	AActor* RootAnchor;
};
