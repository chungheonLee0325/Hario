// Fill out your copyright notice in the Description page of Project Settings.


#include "MarioController.h"

#include "Blueprint/UserWidget.h"
#include "HarioOdyssey/UI/CoinCounterWidget.h"

void AMarioController::BeginPlay()
{
	Super::BeginPlay();
	// UI 생성
	CreateCoinCounterUI();
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
