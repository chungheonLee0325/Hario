﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Mario.h"

#include "GameFramework/InputSettings.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
APlayer_Mario::APlayer_Mario()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//카메라 붐 설정(캐릭터를 따라다니는 카메라)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; //카메라와 캐릭터 간 거리
	CameraBoom->bUsePawnControlRotation=true;//캐릭터 회전을 따라감

	//카메라 설정
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // 카메라 자체 회전 사용 안 함

	// 캐릭터 회전 설정
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true; // 이동 방향으로 회전
	
}

// Called when the game starts or when spawned
void APlayer_Mario::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayer_Mario::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayer_Mario::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 이동 입력
	PlayerInputComponent->BindAxis("OnMoveForward", this, &APlayer_Mario::OnMoveForward);
	PlayerInputComponent->BindAxis("OnMoveRight", this, &APlayer_Mario::OnMoveRight);

	// 카메라 입력
	PlayerInputComponent->BindAxis("OnTurn", this, &APlayer_Mario::OnTurn);
	PlayerInputComponent->BindAxis("OnLookUp", this, &APlayer_Mario::OnLookUp);

	// 점프 입력
	PlayerInputComponent->BindAction("OnStartJump", IE_Pressed, this, &APlayer_Mario::OnStartJump);
	PlayerInputComponent->BindAction("OnStopJump", IE_Released, this, &APlayer_Mario::OnStopJump);
}



//이동 함수
void APlayer_Mario::OnMoveForward(float Value)
{
	if (Controller && Value != 0.0f)
	{
		// 캐릭터 앞 방향 가져오기
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 방향 벡터 계산
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}
void APlayer_Mario::OnMoveRight(float Value)
{
	if (Controller && Value != 0.0f)
	{
		// 캐릭터 오른쪽 방향 가져오기
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 방향 벡터 계산
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

// 카메라 회전 함수
void APlayer_Mario::OnTurn(float Value)
{
	AddControllerYawInput(Value);
}

void APlayer_Mario::OnLookUp(float Value)
{
	AddControllerPitchInput(Value);
}

// 점프 함수
void APlayer_Mario::OnStartJump()
{
	bPressedJump = true;
}


void APlayer_Mario::OnStopJump()
{
	bPressedJump = false;
}