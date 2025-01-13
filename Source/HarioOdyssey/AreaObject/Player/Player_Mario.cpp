// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Mario.h"


#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HarioOdyssey/Projectile/HatProjectile.h"


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

void APlayer_Mario::AddCoin(int32 CoinValue)
{
	CoinCount += CoinValue;

	// UI 업데이트 + 추가 로직
	//OnCoinsUpdate.Broadcast(CoinCount);
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

	//모자 던지기,받기 입력
	PlayerInputComponent->BindAction("OnThrowHat", IE_Pressed, this, &APlayer_Mario::OnThrowHat);
	PlayerInputComponent->BindAction("OnReturnHat", IE_Pressed, this, &APlayer_Mario::OnReturnHat);
	
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

//모자 던지기
void APlayer_Mario::OnThrowHat()
{
	//UE_LOG(LogTemp, Warning, TEXT("Q"));
	 if (bIsHatThrown || !HatProjectileClass) return;
	
	 // 모자 생성 및 던지기
	 FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
	 FRotator SpawnRotation = GetActorRotation();
	 CurrentHat = GetWorld()->SpawnActor<AHatProjectile>(HatProjectileClass, SpawnLocation, SpawnRotation);
	 CurrentHat->InitHatProjectile(this);
	
	 if (CurrentHat)
	 {
	 	UPrimitiveComponent* HatRoot = Cast<UPrimitiveComponent>(CurrentHat->GetRootComponent());
	 	if (HatRoot)
	 	{
	 		FVector LaunchVelocity = GetActorForwardVector() * HatThrowForce;
	 		HatRoot->AddImpulse(LaunchVelocity, NAME_None, true);
	 	}
	
	 	bIsHatThrown = true;
	}
}

//모자 받기
void APlayer_Mario::OnReturnHat()
{
	//UE_LOG(LogTemp, Warning, TEXT("E"));
	if (!bIsHatThrown || !CurrentHat) return;
	
	//모자 반환 상태 설정
	AHatProjectile* Hat = Cast<AHatProjectile>(CurrentHat);
	if (Hat)
	{
		Hat->SetReturning(true);
	}
}

