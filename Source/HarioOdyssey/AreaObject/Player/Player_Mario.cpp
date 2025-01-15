
#include "Player_Mario.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HarioOdyssey/Projectile/HatProjectile.h"
#include "GameFramework/PlayerController.h"

// Sets default values
APlayer_Mario::APlayer_Mario()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    //카메라 붐 설정(캐릭터를 따라다니는 카메라)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f; //카메라와 캐릭터 간 거리
    CameraBoom->bUsePawnControlRotation = true; //캐릭터 회전을 따라감

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
    // OnCoinsUpdate.Broadcast(CoinCount);
}

void APlayer_Mario::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // 이동 입력
    PlayerInputComponent->BindAxis("OnMoveForward", this, &APlayer_Mario::OnMoveForward);
    PlayerInputComponent->BindAxis("OnMoveRight", this, &APlayer_Mario::OnMoveRight);

    // 카메라 입력
    PlayerInputComponent->BindAxis("OnTurn", this, &APlayer_Mario::OnTurn);
    PlayerInputComponent->BindAxis("OnLookUp", this, &APlayer_Mario::OnLookUp);

    PlayerInputComponent->BindAction("OnResetView",IE_Pressed,this, &APlayer_Mario::OnResetView); //시점 초기화

    // 점프 입력
    PlayerInputComponent->BindAction("OnStartJump", IE_Pressed, this, &APlayer_Mario::OnStartJump);

    //모자 던지기,받기 입력
    PlayerInputComponent->BindAction("OnThrowHat", IE_Pressed, this, &APlayer_Mario::OnThrowHat);

    
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

// 카메라 시점 초기화 함수 
void APlayer_Mario::OnResetView() 
{
     UE_LOG(LogTemp,Warning,TEXT("APlayer_Mario::OnResetView"));
    
      if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
      {
         //캐릭터가 바라보는 방향으로 카메라 초기화
         FRotator NewRot=GetActorForwardVector().ToOrientationRotator();
          
         //auto t = PlayerController->GetControlRotation();
         PlayerController->SetControlRotation(NewRot);
      }
}

// 점프 함수
void APlayer_Mario::OnStartJump()
{
    bPressedJump = true;
}



//모자 던지기 및 돌아오는 함수
void APlayer_Mario::OnThrowHat()
{
    if (bIsHatThrown || !HatProjectileClass) return;

    // 모자 생성 위치와 방향 설정
    FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
    FRotator SpawnRotation = GetActorRotation();

    // 모자 액터 생성
    CurrentHat = Cast<AHatProjectile>(GetWorld()->SpawnActor<AActor>(HatProjectileClass, SpawnLocation, SpawnRotation));
    if (CurrentHat)
    {
        // 일정 거리만큼 이동 위치 계산
        FVector TargetLocation = SpawnLocation + GetActorForwardVector() * HatThrowDistance;

        // 모자를 타겟 위치로 이동
        FTimerHandle TimerHandle;
        GetWorldTimerManager().SetTimer(
            TimerHandle,
            FTimerDelegate::CreateLambda([this, TargetLocation]()
            {
                if (CurrentHat)
                {
                    CurrentHat->SetActorLocation(TargetLocation);
                    bIsHatThrown = true; // 모자 던진 상태로 설정

                    // 모자가 던져진 후 일정 시간이 지나면 모자가 돌아오도록 설정
                    FTimerHandle ReturnTimerHandle;
                    GetWorldTimerManager().SetTimer(
                        ReturnTimerHandle,
                        FTimerDelegate::CreateLambda([this, TargetLocation]()
                        {
                            OnReturnHat();
                        }),
                        HatReturnDelay, // 모자가 돌아오는 시간
                        false // 반복 여부 (한 번만 실행)
                    );
                }
            }),
            HatThrowTime, // 모자가 이동하는 시간
            false // 반복 여부 (한 번만 실행)
        );
    }
    }


//모자 받기
void APlayer_Mario::OnReturnHat()
{
    if (CurrentHat && bIsHatThrown)
    {
        FVector StartLocation = CurrentHat->GetActorLocation(); // 모자의 현재 위치
        FVector TargetLocation = GetActorLocation();           // 캐릭터의 현재 위치
        float TravelTime = HatThrowTime;                        // 이동 시간

        // 시간 경과에 따라 모자를 이동시키기 위한 타이머
        FTimerHandle TimerHandle;
        float ElapsedTime = 0.0f;

        // 타이머로 점진적으로 위치를 업데이트
        GetWorldTimerManager().SetTimer(
            TimerHandle,
            FTimerDelegate::CreateLambda([this, StartLocation, TargetLocation, TravelTime, &ElapsedTime]()
            {
                if (CurrentHat)
                {
                    ElapsedTime += GetWorld()->GetDeltaSeconds();

                    // 비율 계산 (0.0 ~ 1.0)
                    float Alpha = FMath::Clamp(ElapsedTime / TravelTime, 0.0f, 1.0f);

                    // 선형 보간
                    FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);
                    CurrentHat->SetActorLocation(NewLocation);

                    // 이동 완료 시 처리
                    if (Alpha >= 1.0f)
                    {
                        CurrentHat->Destroy(); // 모자를 파괴
                        bIsHatThrown = false;  // 모자 던진 상태 초기화
                    }
                }
            }),
            0.01f, // 타이머 업데이트 간격
            true   // 반복 실행
        );
    }
}


