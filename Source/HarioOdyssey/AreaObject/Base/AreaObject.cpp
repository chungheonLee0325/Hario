// Fill out your copyright notice in the Description page of Project Settings.


#include "AreaObject.h"

#include <ThirdParty/ShaderConductor/ShaderConductor/External/DirectXShaderCompiler/include/dxc/DXIL/DxilConstants.h>

#include "HarioOdyssey/AreaObject/Attribute/Condition.h"
#include "HarioOdyssey/AreaObject/Attribute/Health.h"
#include "HarioOdyssey/Contents/HarioGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AAreaObject::AAreaObject()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Health Component 생성
	m_Health = CreateDefaultSubobject<UHealth>(TEXT("Health"));

	// Condition Component 생성
	m_Condition = CreateDefaultSubobject<UCondition>(TEXT("Condition"));
}


// Called when the game starts or when spawned
void AAreaObject::BeginPlay()
{
	Super::BeginPlay();

	// 데이터 초기화
	dt_AreaObject = Cast<UHarioGameInstance>(GetGameInstance())->GetDataAreaObject(m_AreaObjectID);

	// Health 초기화 By Data
	float hpMax = 1.0f;
	if (dt_AreaObject != nullptr)
	{
		hpMax = dt_AreaObject->HPMax;
	}
	
	m_Health->InitHealth(hpMax);
}

void AAreaObject::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

// Called every frame
void AAreaObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAreaObject::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AAreaObject::CalcDamage(float Damage, AActor* Caster, AActor* Target, bool IsPointDamage)
{
	
	if (false == IsPointDamage)
	{
		//범위 피해 적용
		UGameplayStatics::ApplyDamage(Target,
			Damage,
			GetController(),
			this,
			UDamageType::StaticClass());
	}
	else
	{
		//ToDo : 추가 구현
		//UGameplayStatics::ApplyPointDamage();
	}
}

float AAreaObject::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator,
                              AActor* DamageCauser)
{
	//이미 죽었거나 무적 상태인 경우 피해를 받지 않음
	if (IsDie() || m_Condition->HasCondition(EConditionType::Invincible))
		return 0.0f;

	//기본 피해 처리
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	//남은 체력이 0에 도달하면 사망 상태로 전환
	if ( FMath::IsNearlyZero(m_Health->IncreaseHP(-ActualDamage)))
	{
		m_Condition->ExchangeDead();
	}
	
	return ActualDamage;
}

//사망 시
void AAreaObject::OnDie()
{
}

//적 처치 시
void AAreaObject::OnKill()
{
}

//부활 시
void AAreaObject::OnRevival()
{
}

//상태 추가
bool AAreaObject::AddCondition(EConditionType Condition) const
{
	return m_Condition->AddCondition(Condition);
}
//상태 제거
bool AAreaObject::RemoveCondition(EConditionType Condition) const
{
	return m_Condition->RemoveCondition(Condition);
}
//현재 상태
bool AAreaObject::HasCondition(EConditionType Condition) const
{
	return m_Condition->HasCondition(Condition);
}

