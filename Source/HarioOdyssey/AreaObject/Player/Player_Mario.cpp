
#include "Player_Mario.h"

#include "MarioController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HarioOdyssey/Projectile/HatProjectile.h"
#include "GameFramework/PlayerController.h"
#include "HarioOdyssey/UI/CoinCounterWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "HarioOdyssey/AreaObject/Attribute/Health.h"
#include "HarioOdyssey/AreaObject/Monster/Variants/NormalMonsters/ChainChomp/ChainChomp.h"
#include "HarioOdyssey/UI/HealthWidget.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "HarioOdyssey/Utility/TakeDamageMaterial.h"



int APlayer_Mario::m_AreaObjectID = 1;

APlayer_Mario::APlayer_Mario()
{
    PrimaryActorTick.bCanEverTick = true;
   

    //카메라 붐 설정(캐릭터를 따라다니는 카메라)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 300.0f; //카메라와 캐릭터 간 거리
    CameraBoom->bUsePawnControlRotation = true; //캐릭터 회전을 따라감

    //카메라 설정
    CameraComponent  = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    CameraComponent ->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    CameraComponent ->bUsePawnControlRotation = false; // 카메라 자체 회전 사용 안 함

    AutoPossessPlayer = EAutoReceiveInput::Player0;
    PrimaryActorTick.bCanEverTick = true; // 캐릭터가 Tick을 호출하도록 활성화

    // 모자 부착 지점 설정
    HatAttachPoint = CreateDefaultSubobject<USceneComponent>(TEXT("HatAttachPoint"));
    HatAttachPoint->SetupAttachment(GetMesh());

    // 초기화
    // bIsJumping = false;
    // bPressedJump = false;
     CoinCount=0;

    /*모자 [Case3] */
    // 기본값 설정
    bIsHatThrown = false;
    HatSpawnOffset = FVector(100.f, 0.f, 50.f);
    MaxHatThrowDistance = 1000.f;
    HatThrowSpeed = 1200.f;
    HatRecallSpeed = 800.f;

}

void APlayer_Mario::BeginPlay()
{
    Super::BeginPlay();
    
    // MarioController에서 UI 생성 호출
   if ( auto controller = UGameplayStatics::GetPlayerController(this, 0))
   {
       if (auto* MarioController = Cast<AMarioController>(controller))
       {
           CoinCounterWidget=MarioController->CoinCounterWidget;
           HealthWidget = MarioController->HealthWidget;

           if (MarioController)
           {
               CoinCounterWidget->UpdateCoinCounter(CoinCount);
           }
       }
   } 
}

void APlayer_Mario::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    // 점프 중일 때 카메라 위치를 고정
    if (bIsJumping)
    {
        CameraComponent->SetWorldTransform(CameraInitialTransform);
    }

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
    PlayerInputComponent->BindAction("OnStartJump", IE_Released, this, &APlayer_Mario::OnStopJump);

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
    
      if (auto* PC = GetWorld()->GetFirstPlayerController())
      {
         //캐릭터가 바라보는 방향으로 카메라 초기화
         FRotator NewRot=GetActorForwardVector().ToOrientationRotator();
          PC->SetControlRotation(NewRot);
      }
}

// 점프 함수
void APlayer_Mario::OnStartJump()
 {
    bIsJumping = true;

    // 카메라 위치/회전 저장
    CameraInitialTransform = CameraComponent->GetComponentTransform();
}

// 점프 중지 함수
void APlayer_Mario::OnStopJump()
{
    bIsJumping = false;
    
    // 점프 종료 후 카메라를 캐릭터와 동기화
    if (CameraComponent)
    {
        CameraComponent->AttachToComponent(CameraBoom, FAttachmentTransformRules::KeepRelativeTransform);
    }
}

//모자 던지기 및 돌아오는 함수
void APlayer_Mario::OnThrowHat()
{
    if (HatClass) // HatClass는 HatProjectile 클래스의 레퍼런스
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this; // 소유자를 마리오로 설정

        // 모자의 시작 위치와 방향
        FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 200.0f;
        FRotator SpawnRotation = GetActorRotation();

        // HatProjectile 생성
        AHatProjectile* Hat = GetWorld()->SpawnActor<AHatProjectile>(HatClass, SpawnLocation, SpawnRotation, SpawnParams);
        if (Hat)
        {
            // 모자에 필요한 초기 설정 전달 (예: 속도, 방향)
            Hat->InitializeHat(GetActorForwardVector(), this);
        }
    }
    
}

//동전 획득
void APlayer_Mario::AddCoin(int32 CoinValue)
{
    CoinCount += CoinValue;
    // UI Coin 업데이트 
    if (CoinCounterWidget)
    {
    	CoinCounterWidget->UpdateCoinCounter(CoinCount);
    }
}
    
//데미지 처리
float APlayer_Mario::TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator,
    AActor* DamageCauser)
{
    	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
    	if (FMath::IsNearlyZero(ActualDamage))
    	{
    	    return ActualDamage; // 이미 죽었거나 무적일 때
    	}

    	// 무적 상태 적용
    	if (AddCondition(EConditionType::Invincible))
    	{
    	    UE_LOG(LogTemp, Warning, TEXT("무적상태 진입"));
    	    TakeDamageMaterialHandler();  
    	    if (TakeDamageMaterialInstance)
    	    {
    	        TakeDamageMaterialInstance->StartBlinkEffect(GetMesh(), BlinkTimerHandle, InvincibleLocalTimerHandle);
    	    }
    	}

    	return ActualDamage;
   
}

    
/** Hit 이벤트 (체인촘프 등과 부딪혔을 때) */
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

/** 무적 해제 */
void APlayer_Mario::RemoveInvincibility()
{
    RemoveCondition(EConditionType::Invincible);
}

void APlayer_Mario::TakeDamageMaterialHandler()
{
    if (!TakeDamageMaterialInstance)
    {
    	TakeDamageMaterialInstance = NewObject<UTakeDamageMaterial>();
    }
}







