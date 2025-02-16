﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

UCLASS()
class HARIOODYSSEY_API ABaseItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseItem();

	virtual bool CanBeCollectedBy(class APlayer_Mario* Player);
	virtual void OnCollected(class APlayer_Mario* Player);
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	// 아이템 효과 적용(자식 클래스에서 구현)
	virtual void ApplyEffect(class APlayer_Mario* Player) PURE_VIRTUAL(ABaseItem::ApplyEffect,);

	UPROPERTY(EditAnywhere, Category = "Collection")
	USoundBase* CollectionSound;

	UPROPERTY(EditAnywhere, Category = "Collection")
	UParticleSystem* CollectionEffect;

	UPROPERTY(EditAnywhere, Category = "Collection")
	class USphereComponent* CollectionSphere;

	UPROPERTY(EditAnywhere, Category = "Collection")
	class UStaticMeshComponent* ItemMesh;

	UFUNCTION()
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
							  AActor* OtherActor,
							  UPrimitiveComponent* OtherComp,
							  int32 OtherBodyIndex,
							  bool bFromSweep,
							  const FHitResult& SweepResult);

	bool m_IsCollected;

	// 아이템 진동 라디안
	UPROPERTY(EditAnywhere)
	float Radian;
	// 아이템 진동 진폭
	UPROPERTY(EditAnywhere)
	float Amplitude;
	// 아이템 진동 주기
	UPROPERTY(EditAnywhere)
	float Period;
	// 아이템 회전 속도
	FRotator ItemRotator = FRotator(0, 100.f , 0);

};
