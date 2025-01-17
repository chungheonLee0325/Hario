// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MarioController.generated.h"

class UCoinCounterWidget;
/**
 * 
 */
UCLASS()
class HARIOODYSSEY_API AMarioController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:
	
	// CoinCounterWidget UI 생성
	void CreateCoinCounterUI();
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UI")
	TSubclassOf<UCoinCounterWidget> CoinCounterWidgetClass;

	TObjectPtr<UCoinCounterWidget> CoinCounterWidget;
	

	int32 CoinCount = 0;
	
	//HealthWidget UI 생성
	void CreateHealthUI();

	//????????수정필요
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateHealthUI(float CurrentHealth);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="UI")
	TSubclassOf<class UHealthWidget> HealthWidgetClass;

	TObjectPtr<UHealthWidget>HealthWidget;
	

	
};
