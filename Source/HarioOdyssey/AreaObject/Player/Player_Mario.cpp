
#include "Player_Mario.h"

#include "MarioController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HarioOdyssey/Projectile/HatProjectile.h"
#include "GameFramework/PlayerController.h"
#include "HarioOdyssey/UI/CoinCounterWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "HarioOdyssey/AreaObject/Attribute/Health.h"
#include "HarioOdyssey/AreaObject/Monster/Variants/NormalMonsters/ChainChomp/ChainChomp.h"
#include "HarioOdyssey/UI/HealthWidget.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APlayer_Mario::APlayer_Mario()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    m_AreaObjectID = 1;

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

    
    //m_Condition = CreateDefaultSubobject<AAreaObject>(TEXT("Condition"));
   
}

// Called when the game starts or when spawned
void APlayer_Mario::BeginPlay()
{
    Super::BeginPlay();
    
    // MarioController에서 UI 생성 호출
    auto controller = UGameplayStatics::GetPlayerController(this, 0);
    auto* MarioController = Cast<AMarioController>(controller);
    if (MarioController)
    {
        CoinCounterWidget=MarioController->CoinCounterWidget;
        HealthWidget = MarioController->HealthWidget;

        CoinCounterWidget->UpdateCoinCounter(CoinCount);
    }
    
 
}

// Called every frame
void APlayer_Mario::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    //무적일때 깜빡이기
    // if (!bIsInvincible) //????????데미지 받으면
    // {
    //     float TimeInSenconds = GetWorld()->GetTimeSeconds();
    //     bool bShouldBlink=FMath::Fmod(TimeInSenconds*5.0f,1.0f) > 0.5f;
    //     SetActorHiddenInGame(bShouldBlink);
    // }
    // else
    // {
    //     SetActorHiddenInGame(false);
    // }
}



void APlayer_Mario::AddCoin(int32 CoinValue)
{
    CoinCount += CoinValue;
    // UI Coin 업데이트 
    if (CoinCounterWidget)
    {
        CoinCounterWidget->UpdateCoinCounter(CoinCount);
    }
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
     //UE_LOG(LogTemp,Warning,TEXT("시점초기화"));
    
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
   
    if (HatClass) // HatClass는 HatProjectile 클래스의 레퍼런스
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this; // 소유자를 마리오로 설정

        // 모자의 시작 위치와 방향
        FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 100.0f;
        FRotator SpawnRotation = GetActorRotation();

        // HatProjectile 생성
        AHatProjectile* Hat = GetWorld()->SpawnActor<AHatProjectile>(HatClass, SpawnLocation, SpawnRotation, SpawnParams);
        if (Hat)
        {
            // 모자에 필요한 초기 설정 전달 (예: 속도, 방향)
            Hat->InitializeHat(GetActorForwardVector());
        }
    }
}

float APlayer_Mario::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator,
    AActor* DamageCauser)
{

    auto actureDamage= Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
    // 무적이거나, 이미 죽은 경우
    if (FMath::IsNearlyZero(actureDamage))
    {
        return actureDamage;
    }
    //무적 상태 적용
    if (AddCondition(EConditionType::Invincible))
    {
        UE_LOG(LogTemp,Warning,TEXT("무적상태"));
        FTimerHandle TimerHandle;
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
        {
            // 딜레이 후 실행될 코드 - 무적 해제
            RemoveCondition(EConditionType::Invincible);
            UE_LOG(LogTemp,Warning,TEXT("무적해제"));
        }, 2.0f, false); // 2초 후 실행, 반복 안함
    }

    return actureDamage;
   


    
    // 깜빡이는 효과 시작
    // if (ActualDamage > 0.0f && !bIsInvincible)
    // {
    //     HandleBlinkingEffect(true); 
    // }

 
}

void APlayer_Mario::NotifyHit(UPrimitiveComponent* MyComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                             bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
                             const FHitResult& Hit)
{
    Super::NotifyHit(MyComp, OtherActor, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);

    if (OtherActor && OtherActor->IsA(AChainChomp::StaticClass()))
    {
        AddCondition(EConditionType::Invincible);

        GetWorldTimerManager().SetTimer(InvincibleTimerHandle, this, &APlayer_Mario::RemoveInvincibility, 3.0f, false);
    }
}

void APlayer_Mario::RemoveInvincibility()
{
    RemoveCondition(EConditionType::Invincible);
}




// 깜빡이는 효과 처리 및 정리
// void APlayer_Mario::StartBlinkingEffect()
// {
//     HandleBlinkingEffect(true); // 래퍼 함수
// }
// void APlayer_Mario::HandleBlinkingEffect(bool bStart)
// {
//     if (bStart)
//     {
//         // 무적 상태 활성화
//         bIsInvincible = true;
//
//         // 초기 타이머 설정
//         BlinkTimer = 0.0f;
//         BlinkDuration = 2.0f;   // 총 지속 시간
//         BlinkInterval = 0.2f;   // 깜빡이는 간격
//
//         // 타이머 시작
//         GetWorldTimerManager().SetTimer(BlinkHandle, this, &APlayer_Mario::StartBlinkingEffect, BlinkInterval, true);
//
//     }
//     else
//     {
//         // 타이머 실행 중일 때 깜빡임 처리
//         if (USkeletalMeshComponent* LocalMesh  = GetMesh())
//         {
//             // Mesh의 보이기 상태를 반전
//             bool bNewVisibility = !LocalMesh ->IsVisible();
//             LocalMesh ->SetVisibility(bNewVisibility);
//         }
//
//         // 타이머 업데이트
//         BlinkTimer += BlinkInterval;
//
//         // 깜빡이는 효과 종료 조건
//         if (BlinkTimer >= BlinkDuration)
//         {
//             // 타이머 정지 및 초기화
//             GetWorldTimerManager().ClearTimer(BlinkHandle);
//
//             if (USkeletalMeshComponent* LocalMesh  = GetMesh())
//             {
//                 // Mesh를 항상 보이도록 설정
//                 LocalMesh ->SetVisibility(true);
//             }
//
//             // 무적 상태 비활성화
//             bIsInvincible = false;
//         }
//     }
//
//   


    
    // ToDoKHA : 깜빡이기 + 무적
    //m_Condition->AddCondition(EConditionType::Invincible);

    // ToDoKHA : 애니메이션(동작 불가 n초)
    //return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
//}

//
// void APlayer_Mario::OnDie()
// {
//     Super::OnDie();
// }

