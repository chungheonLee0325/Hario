// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CoinCounterWidget.generated.h"

/**
 * 
 */
UCLASS()
class HARIOODYSSEY_API UCoinCounterWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateCoinCounter(int32 NewCoinCount);
	void UpdateRegionCoinCounter(int32 NewCoinCount);
	void NativeConstruct() override;

protected:
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* CoinCountText;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* RegionCoinCountText;
};
