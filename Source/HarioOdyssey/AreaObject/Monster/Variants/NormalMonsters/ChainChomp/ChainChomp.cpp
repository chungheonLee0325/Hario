// Fill out your copyright notice in the Description page of Project Settings.


#include "ChainChomp.h"

#include "RootAnchor.h"
#include "ChainComponent.h"
#include "Components/SphereComponent.h"
#include "HarioOdyssey/AreaObject/Monster/AI/Derived/AiMonster/ChainChomp/AiChainChomp.h"
#include "HarioOdyssey/AreaObject/Skill/Monster/ChainChompLaunch.h"
#include "HarioOdyssey/AreaObject/Skill/Monster/ChainChompPull.h"
#include "HarioOdyssey/Utility/SpawnUtilLib.h"
#include "HarioOdyssey/AreaObject/Skill/Monster/ChainChompPullAndLaunchSkill.h"
#include "HarioOdyssey/Objects/_Components/DestructComponent.h"
#include "HarioOdyssey/PathMover/VerticalMover.h"


// Sets default values
AChainChomp::AChainChomp()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempCCBody(TEXT(
		"/Script/Engine.SkeletalMesh'/Game/_Resource/Monster/ChainChomp/ChainChomp2/ChainChomp2.ChainChomp2'"));
	if (tempCCBody.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempCCBody.Object);
	}
	GetMesh()->SetRelativeScale3D(FVector(0.3f));
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 40.0f), FRotator(0.0f, -90.0f, 0.0f));

	ChainChompSphere = CreateDefaultSubobject<USphereComponent>("ChainChompSphere");
	ChainChompSphere->SetupAttachment(GetMesh());
	ChainChompSphere->SetRelativeScale3D(FVector(14.0f));
	ChainChompSphere->SetCollisionProfileName(TEXT("Enemy"));

	ChainStartScene = CreateDefaultSubobject<USceneComponent>("ChainStartScene");
	ChainStartScene->SetupAttachment(GetMesh());
	ChainStartScene->SetRelativeLocationAndRotation(FVector(0.0f, -635.f, 0.0f), FRotator(0.0f, -90.0f, 0.0f));

	ChainComponent = CreateDefaultSubobject<UChainComponent>("Chains");
	ChainComponent->SetupAttachment(ChainStartScene);
	//ChainComponent->AttachToComponent(ChainStartScene, FAttachmentTransformRules::KeepRelativeTransform);
	ChainComponent->RootScene->SetupAttachment(ChainComponent);
	ChainComponent->InitAttach();

	// 잔상용 투명 마테리얼 셋팅
	{
		ConstructorHelpers::FObjectFinder<UMaterial> tempMaterial(TEXT(
			"/Script/Engine.Material'/Game/_Resource/Monster/ChainChomp/ChainChomp2/mat_T_wanwan_body.mat_T_wanwan_body'"));
		if (tempMaterial.Succeeded())
		{
			GhostTrailMaterials.Add(tempMaterial.Object);
		}
		ConstructorHelpers::FObjectFinder<UMaterial> tempMaterial2(TEXT(
			"/Script/Engine.Material'/Game/_Resource/Monster/ChainChomp/ChainChomp2/mat_ET_wanwan_body.mat_ET_wanwan_body'"));
		if (tempMaterial2.Succeeded())
		{
			GhostTrailMaterials.Add(tempMaterial2.Object);
		}
		ConstructorHelpers::FObjectFinder<UMaterial> tempMaterial3(TEXT(
			"/Script/Engine.Material'/Game/_Resource/Monster/ChainChomp/ChainChomp2/mat_T_wanwan_eye.mat_T_wanwan_eye'"));
		if (tempMaterial3.Succeeded())
		{
			GhostTrailMaterials.Add(tempMaterial3.Object);
		}
		ConstructorHelpers::FObjectFinder<UMaterial> tempMaterial4(TEXT(
			"/Script/Engine.Material'/Game/_Resource/Monster/ChainChomp/ChainChomp2/mat_ET_wanwan_eye.mat_ET_wanwan_eye'"));
		if (tempMaterial4.Succeeded())
		{
			GhostTrailMaterials.Add(tempMaterial4.Object);
		}
	}

	// 스킬 추가
	//m_StateSkillClasses.Add(EAiStateType::Attack, UChainChompPullAndLaunchSkill::StaticClass());
	m_StateSkillClasses.Add(EAiStateType::Attack, UChainChompPull::StaticClass());
	m_SkillClasses.Add(UChainChompLaunch::StaticClass());
	m_SkillClasses.Add(UChainChompPull::StaticClass());

	m_AiFSM = AChainChomp::CreateFSM();
}

// Called when the game starts or when spawned
void AChainChomp::BeginPlay()
{
	Super::BeginPlay();

	RootPosition = GetActorLocation();

	RootAnchor = USpawnUtilLib::SpawnActorOnGround(this, ARootAnchor::StaticClass(), RootPosition,
	                                               FRotator::ZeroRotator);
	if (RootAnchor != nullptr)
	{
		RootAnchorPosition = RootAnchor->GetActorLocation();
	}
	ChainComponent->UpdateChainPosition(ChainStartScene->GetComponentLocation(),
	                                    RootAnchorPosition + FVector(0, 0, 60));

	ChainChompSphere->OnComponentBeginOverlap.AddDynamic(this, &AChainChomp::OnBodyBeginOverlap);

	AddCondition(EConditionType::Invincible);
}

UBaseAiFSM* AChainChomp::CreateFSM()
{
	return CreateDefaultSubobject<UAiChainChomp>("FSM");
}

// Called every frame
void AChainChomp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//if (m_PathMover->IsMoving() || m_PathMover->IsRotating())
	{
		ChainComponent->UpdateChainPosition(ChainStartScene->GetComponentLocation(),
		                                    RootAnchorPosition + FVector(0, 0, 60));
	}
}

void AChainChomp::OnDie()
{
	Super::OnDie();

	RootAnchor->Destroy();
}

// Called to bind functionality to input
void AChainChomp::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool AChainChomp::CanBeCaptured_Implementation()
{
	OnCaptureStart_Implementation();
	return true;
}

void AChainChomp::OnCaptureStart_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("OnCaptureStart_Implementation"));
}

void AChainChomp::OnCaptureEnd_Implementation()
{
	ICapturable::OnCaptureEnd_Implementation();
}

void AChainChomp::WhileCaptured_Implementation(ACharacter* CaptureOwner)
{
	ICapturable::WhileCaptured_Implementation(CaptureOwner);
}

void AChainChomp::OnBodyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                     const FHitResult& SweepResult)
{
	Super::OnBodyBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (true == IsDestructDmgAble)
	{
		auto monster = Cast<ABaseMonster>(OtherActor);
		if (monster != nullptr)
		{
			CalcDamage(1.0f, this, monster);
		}

		auto dComp = OtherActor->FindComponentByClass<UDestructComponent>();
		if (dComp != nullptr)
		{
			dComp->ApplyDestruction(GetActorLocation());

			// 움직임 정지
			StopAll();
			m_VerticalMover->StopVerticalMovement();

			FVector dir = (GetActorLocation() - OtherActor->GetActorLocation()).GetSafeNormal2D();
			MoveToLocation(GetActorLocation() + dir * 200.f, 1, EMovementInterpolationType::Linear);
			m_VerticalMover->StartVerticalMovement(GetMesh(), 250.f, 0.5f, 0.2f);

			Cast<UChainChompLaunch>(m_CurrentSkill)->BlockByObject();
		}
	}
}

void AChainChomp::StartVibration(float Intensity, float Duration, float Interval)
{
	// 진동 강도와 지속 시간 설정
	VibrationIntensity = Intensity;
	VibrationDuration = Duration;
	VibrationInterval = Interval;

	// 타이머 시작 (Interval 초 간격으로 진동 적용)
	TWeakObjectPtr<AChainChomp> WeakThis = this;
	GetWorld()->GetTimerManager().SetTimer(VibrationTimerHandle, [WeakThis]()
	{
		if (auto* StrongThis = WeakThis.Get())
		{
			// 현재 위치 가져오기
			FVector CurrentLocation = StrongThis->GetActorLocation();

			// 랜덤한 방향으로 진동 적용
			FVector RandomOffset = FVector(
				FMath::FRandRange(-StrongThis->VibrationIntensity, StrongThis->VibrationIntensity),
				FMath::FRandRange(-StrongThis->VibrationIntensity, StrongThis->VibrationIntensity),
				FMath::FRandRange(-StrongThis->VibrationIntensity, StrongThis->VibrationIntensity)
			);

			// 새로운 위치 설정
			FVector NewLocation = CurrentLocation + RandomOffset;
			StrongThis->SetActorLocation(NewLocation);

			// 지속 시간 감소
			StrongThis->VibrationDuration -= StrongThis->VibrationInterval;

			// 지속 시간이 끝나면 진동 중지
			if (StrongThis->VibrationDuration <= 0.0f)
			{
				StrongThis->StopVibration();
			}
		}
	}, Interval, true);
}

void AChainChomp::StopVibration()
{
	// 타이머 중지
	GetWorld()->GetTimerManager().ClearTimer(VibrationTimerHandle);
}



TArray<UMaterialInterface*> AChainChomp::GetGhostTrailMaterials() const
{
	return GhostTrailMaterials;
}
