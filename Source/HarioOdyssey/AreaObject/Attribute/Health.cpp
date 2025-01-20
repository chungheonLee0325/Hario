// Fill out your copyright notice in the Description page of Project Settings.


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
	m_HPMax = _hpMax; //최대 체력을 설정
	m_HP = m_HPMax; //현재 체력을 최대 체력으로 초기화
}

//체력을 증가,감소
float UHealth::IncreaseHP(float Delta)
{
	//체력값을 최소0과 최대 체력 사이로 클램핑
	m_HP = FMath::Clamp(m_HP + Delta,0.0f,m_HPMax);
	return m_HP; //변경된 체력 반환
}

void UHealth::SetHPByRate(float Rate)
{
	m_HP = m_HPMax * Rate; //최대 체력 비율화
}

float UHealth::GetHP() const
{
	return m_HP; //현재 체력 반환
}

