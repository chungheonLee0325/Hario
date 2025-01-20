// Fill out your copyright notice in the Description page of Project Settings.


#include "ChainComponent.h"

#include "MovieSceneSequenceID.h"


// Sets default values for this component's properties
UChainComponent::UChainComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	ChainLinks.SetNum(ChainNum);
	for(int32 i = 0; i < ChainNum; i++)
	{
		FString CompName = FString::Printf(TEXT("ChainLink_%d"), i);
		ChainLinks[i] = CreateDefaultSubobject<UStaticMeshComponent>(*CompName);
		ChainLinks[i]->SetRelativeScale3D(FVector(0.8f));
		ChainLinks[i]->SetupAttachment(this);
	}
	
	// 체인 링크 메시 초기화
	ConstructorHelpers::FObjectFinder<UStaticMesh> tempMesh(TEXT("/Script/Engine.StaticMesh'/Game/_Resource/Monster/ChainChomp/Chain/ST_Chain.ST_Chain'"));
	if (tempMesh.Succeeded())
	{
		for(int32 i = 0; i < ChainNum; i++)
		{
			ChainLinks[i]->SetStaticMesh(tempMesh.Object);
		}
	}
}


// Called when the game starts
void UChainComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UChainComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UChainComponent::UpdateChainPosition(FVector Start, FVector End)
{
	// 시작점과 끝점 사이에 체인 링크들을 배치
	FVector Direction = End - Start;
	float SegmentLength = Direction.Size() / (ChainLinks.Num());
	Direction.Normalize();

	for(int32 i = 0; i < ChainLinks.Num(); i++)
	{
		FVector Position = Start + Direction * (SegmentLength * i);
		ChainLinks[i]->SetWorldLocation(Position);

		// 체인 링크의 회전 설정
		FRotator Rotation = FRotationMatrix::MakeFromX(Direction).Rotator();
		ChainLinks[i]->SetWorldRotation(Rotation);
	}
}