// Fill out your copyright notice in the Description page of Project Settings.


#include "CoinCounterWidget.h"

#include "Components/TextBlock.h"

void UCoinCounterWidget::UpdateCoinCounter(int32 NewCoinCount)
{
	if (CoinCountText)
	{
		CoinCountText->SetText(FText::AsNumber(NewCoinCount));
	}
}

void UCoinCounterWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (CoinCountText)
	{
		CoinCountText->SetText(FText::FromString(TEXT("0")));
	}
}
