// Fill out your copyright notice in the Description page of Project Settings.


#include "ChainChomp.h"

#include "RootAnchor.h"
#include "ChainComponent.h"
#include "Components/SphereComponent.h"
#include "Components/SplineComponent.h"
#include "HarioOdyssey/AreaObject/Monster/AI/Derived/AiMonster/ChainChomp/AiChainChomp.h"
#include "HarioOdyssey/Utility/SpawnUtilLib.h"
#include "HarioOdyssey/AreaObject/Skill/Monster/ChainChompPullAndLaunchSkill.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AChainChomp::AChainChomp()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;



	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempCCBody(TEXT("/Script/Engine.SkeletalMesh'/Game/_Resource/Monster/ChainChomp/ChainChomp2/ChainChomp2.ChainChomp2'"));
	if (tempCCBody.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempCCBody.Object);
	}
	GetMesh()->SetRelativeScale3D(FVector(0.3f));
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f,0.0f,40.0f),FRotator(0.0f,-90.0f,0.0f));
	
	ChainChompSphere = CreateDefaultSubobject<USphereComponent>("ChainChompSphere");
	ChainChompSphere->SetupAttachment(GetMesh());
	ChainChompSphere->SetRelativeScale3D(FVector(14.0f));
	ChainChompSphere->SetCollisionProfileName(TEXT("Enemy"));

	ChainStartScene = CreateDefaultSubobject<USceneComponent>("ChainStartScene");
	ChainStartScene->SetupAttachment(GetMesh());
	ChainStartScene->SetRelativeLocationAndRotation(FVector(0.0f,-635.f,0.0f),FRotator(0.0f,-90.0f,0.0f));

	ChainComponent = CreateDefaultSubobject<UChainComponent>("ChainComponent");
	ChainComponent->SetupAttachment(ChainStartScene);
	
	StateSkillMap.Add(EAiStateType::Attack,UChainChompPullAndLaunchSkill::StaticClass());

	m_AiFSM = AChainChomp::CreateFSM();
}

// Called when the game starts or when spawned
void AChainChomp::BeginPlay()
{
	Super::BeginPlay();
	
	RootPosition = GetActorLocation();	
	FVector SpawnLocation = RootPosition;
	AActor* RootAnchor =USpawnUtilLib::SpawnActorOnGround(this,ARootAnchor::StaticClass(),SpawnLocation,FRotator::ZeroRotator);
	if (RootAnchor != nullptr)
	{
		RootAnchorPosition = RootAnchor->GetActorLocation();
	}
	ChainComponent->UpdateChainPosition(ChainStartScene->GetComponentLocation(),RootAnchorPosition +FVector(0,0,60));

	ChainChompSphere->OnComponentBeginOverlap.AddDynamic(this, &AChainChomp::OnBodyBeginOverlap);
}

UBaseAiFSM* AChainChomp::CreateFSM()
{
	return CreateDefaultSubobject<UAiChainChomp>("FSM");
}

// Called every frame
void AChainChomp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (m_PathMover->IsMoving() || m_PathMover->IsRotating())
	{
		ChainComponent->UpdateChainPosition(ChainStartScene->GetComponentLocation(),RootAnchorPosition +FVector(0,0,60));
	}
}

// Called to bind functionality to input
void AChainChomp::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

bool AChainChomp::CanBeCaptured_Implementation()
{
	return ICapturable::CanBeCaptured_Implementation();
}

void AChainChomp::OnCaptureStart_Implementation()
{
	ICapturable::OnCaptureStart_Implementation();
}

void AChainChomp::OnCaptureEnd_Implementation()
{
	ICapturable::OnCaptureEnd_Implementation();
}

void AChainChomp::WhileCaptured_Implementation(ACharacter* CaptureOwner)
{
	ICapturable::WhileCaptured_Implementation(CaptureOwner);
}

