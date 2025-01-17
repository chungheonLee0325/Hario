// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthWidget.h"
#include "Components/TextBlock.h"


void UHealthWidget::UpdateHealth(float m_HP)
{
	if (HealthText)
	{
		HealthText->SetText(FText::AsNumber(m_HP));
	}
}



void UHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (HealthText)
	{
		HealthText->SetText(FText::FromString(TEXT("")));
	}
}
