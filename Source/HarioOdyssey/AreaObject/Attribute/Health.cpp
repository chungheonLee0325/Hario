﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Health.h"

#include "VectorUtil.h"
#include "HarioOdyssey/AreaObject/Base/AreaObject.h"


// Sets default values for this component's properties
UHealth::UHealth()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// ...
}


// Called when the game starts
void UHealth::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealth::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealth::InitHealth(float _hpMax)	// 추후 사용에따라 구조체나 다른 형식으로 변환 예정
{
	m_HPMax = _hpMax;
	m_HP = m_HPMax;
}

float UHealth::IncreaseHP(float Delta)
{
	m_HP = FMath::Clamp(m_HP + Delta,0.0f,m_HPMax);
	return m_HP;
}

void UHealth::SetHPByRate(float Rate)
{
	m_HP = m_HPMax * Rate;
}

float UHealth::GetHP() const
{
	return m_HP;
}
