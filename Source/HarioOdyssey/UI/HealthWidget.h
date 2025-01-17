// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class HARIOODYSSEY_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()

	
public:
	UFUNCTION(BlueprintCallable, Category = "Health")
	void UpdateHealth(float CurrentHealth);
	void NativeConstruct() override;
    
protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* HealthText; // 위젯의 텍스트 바인딩
};

