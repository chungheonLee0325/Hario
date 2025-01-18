// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthWidget.h"
#include "Components/TextBlock.h"
#include "HarioOdyssey/AreaObject/Base/AreaObject.h"

// ToDo : 삭제예정
//void UHealthWidget::UpdateHealth(float m_HP)
//{
//	//if (HealthText)
//	//{
//	//	HealthText->SetText(FText::AsNumber(m_HP));
//	//}
//}
void UHealthWidget::UpdateHP(float currentHP, float delta, float maxHP)
{
	if (HealthText)
	{
		HealthText->SetText(FText::AsNumber(currentHP));
	}
}


void UHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (HealthText)
	{
		HealthText->SetText(FText::FromString(TEXT("")));
	}
	if (APlayerController* PC = GetOwningPlayer())
	{
		if (AAreaObject* Character = Cast<AAreaObject>(PC->GetPawn()))
		{
			Character->m_Health->OnHealthChanged.AddDynamic(this, &UHealthWidget::UpdateHP);
		}
	}
}
