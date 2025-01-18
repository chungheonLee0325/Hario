// Fill out your copyright notice in the Description page of Project Settings.


#include "MarioController.h"

#include "HarioOdyssey/UI/CoinCounterWidget.h"
#include "HarioOdyssey/UI/HealthWidget.h"


void AMarioController::BeginPlay()
{
	Super::BeginPlay();
	// UI 생성
	CreateCoinCounterUI();
	CreateHealthUI();
}


void AMarioController::CreateCoinCounterUI()
{
	if (CoinCounterWidgetClass && !CoinCounterWidget)
	{
		CoinCounterWidget = CreateWidget<UCoinCounterWidget>(GetWorld(), CoinCounterWidgetClass);
		if (CoinCounterWidget)
		{
			CoinCounterWidget->AddToViewport();
			CoinCounterWidget->UpdateCoinCounter(CoinCount);
		}
	}
}


void AMarioController::CreateHealthUI()
{
	if (HealthWidgetClass && !HealthWidget)
	{
		HealthWidget = CreateWidget<UHealthWidget>(GetWorld(), HealthWidgetClass);
		if (HealthWidget)
		{
			HealthWidget->AddToViewport();
			//HealthWidget->UpdateHealth(CurrentHealth);
		}
	}
}

// ToDo : 삭제예정
void AMarioController::UpdateHealthUI(float CurrentHealth)
{
	//if (HealthWidget)
	//{
	//	HealthWidget->UpdateHealth(CurrentHealth);
	//}
}