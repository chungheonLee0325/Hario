
#include "Player_Mario.h"

#include "MarioController.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "HarioOdyssey/Projectile/HatProjectile.h"
#include "GameFramework/PlayerController.h"
#include "HarioOdyssey/UI/CoinCounterWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "HarioOdyssey/AreaObject/Monster/Variants/NormalMonsters/ChainChomp/ChainChomp.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "HarioOdyssey/Contents/HarioGameMode.h"
#include "HarioOdyssey/Utility/TakeDamageMaterial.h"
#include "Particles/ParticleSystemComponent.h"


APlayer_Mario::APlayer_Mario()
{
    PrimaryActorTick.bCanEverTick = true;

    // AreaObjectID Set
    m_AreaObjectID = 1;
    
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

 
     CoinCount=0;

  
    // 기본값 설정
    bIsHatThrown = false;
    HatInstance =nullptr;

    WornHatMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WornHatMesh"));
    WornHatMesh->SetupAttachment(GetMesh(), "HeadSocket");
    HatSpawnOffset = FVector(100.f, 0.f, 50.f);
    MaxHatThrowDistance = 300.f;
    HatThrowSpeed = 1200.f;
    HatRecallSpeed = 800.f;

}

void APlayer_Mario::BeginPlay()
{
    Super::BeginPlay();

    GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &APlayer_Mario::OnBodyBeginOverlap);
    
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
    
    // 점프 중일 때 카메라 위치 고정
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

    PlayerInputComponent->BindAction("OnSpinHat", IE_Pressed, this, &APlayer_Mario::OnSpinHat);
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
    ACharacter::Jump(); // 기본 제공되는 점프 함수 호출
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
    ACharacter::StopJumping(); // 기본 제공되는 점프 종료 함수 호출
}

//모자 던지기 및 돌아오는 함수
void APlayer_Mario::OnThrowHat() //직선이동
{
    // 모자가 이미 던져졌다면 새로운 모자를 생성하지 않음
    if (bIsHatThrown || HatInstance != nullptr)
    {
        return;
    }
    if (HatClass) // HatClass는 HatProjectile 클래스의 레퍼런스
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this; // 소유자를 마리오로 설정

        // 모자의 시작 위치와 방향
        FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 5.0f;
        FRotator SpawnRotation = GetActorRotation();

        // HatProjectile 생성
        HatInstance = GetWorld()->SpawnActor<AHatProjectile>(HatClass, SpawnLocation, SpawnRotation, SpawnParams);
        if (HatInstance)
        {
            bIsHatThrown = true;
            // 모자에 필요한 초기 설정 전달 (예: 속도, 방향)
            HatInstance->InitializeHat(GetActorForwardVector(), this, false);
            
            // 모자 던질 때 골뱅이 효과 활성화
            //HatInstance->bExpanding = true;
            
            HatInstance->OnDestroyed.AddDynamic(this, &APlayer_Mario::OnHatReturned);
            
            WornHatMesh->SetVisibility(false);
           //UE_LOG(LogTemp, Warning, TEXT("모자 생성 성공"));
        }
    }
    
}
void APlayer_Mario::OnSpinHat() // 나선형 이동
{
    if (bIsHatThrown || HatInstance != nullptr) return;

    if (HatClass)
    {
        FActorSpawnParameters SpawnParams;
        SpawnParams.Owner = this;

        FVector SpawnLocation = GetActorLocation() + GetActorForwardVector() * 5.0f;
        FRotator SpawnRotation = GetActorRotation();

        HatInstance = GetWorld()->SpawnActor<AHatProjectile>(HatClass, SpawnLocation, SpawnRotation, SpawnParams);
        if (HatInstance)
        {
            bIsHatThrown = true;
            HatInstance->InitializeHat(GetActorForwardVector(), this, true); // true = 나선형 던지기
            HatInstance->OnDestroyed.AddDynamic(this, &APlayer_Mario::OnHatReturned);
            WornHatMesh->SetVisibility(false);
        }
    }
}
void APlayer_Mario::OnHatReturned(AActor* DestroyedActor)
{
    bIsHatThrown = false;

    if (HatInstance == DestroyedActor)
    {
        HatInstance = nullptr;
        WornHatMesh->SetVisibility(true);
        UE_LOG(LogTemp, Warning, TEXT("모자가 돌아왔습니다."));
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("HatInstance가 DestroyedActor와 일치하지 않음"));
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

void APlayer_Mario::AddRegionCoin(int32 CoinValue)
{
    RegionCoinCount += CoinValue;
    // UI Coin 업데이트 
    if (CoinCounterWidget)
    {
        CoinCounterWidget->UpdateRegionCoinCounter(RegionCoinCount);
    }
}

void APlayer_Mario::AddStarEffect(float LifeTime)
{
    // LifeTime 동안 무적 적용
    if (true == HasCondition(EConditionType::Invincible))
    {
        RemoveCondition(EConditionType::Invincible);
        GetWorld()->GetTimerManager().ClearTimer(InvincibleTimerHandle);

        // ToDo : 마테리얼 Handle 제거
    }
    AddCondition(EConditionType::Invincible);

    // LifeTime 동안 충돌시 대상 die + 대상 무적 제거
    IsStarEffecting = true;

    // 이펙트 추가, Sound 추가
    StarEffectsParticleComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(StarEffectsParticle, GetMesh(),EName::Actor,FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f),EAttachLocation::SnapToTarget,false);
    Cast<AHarioGameMode>(GetWorld()->GetAuthGameMode())->SwitchBGMTemporary(101,12.f);
    
    TWeakObjectPtr<APlayer_Mario> WeakThis = this;
    GetWorld()->GetTimerManager().SetTimer(InvincibleTimerHandle, [WeakThis]()
    {
        if (auto* StrongThis = WeakThis.Get())
        {
            StrongThis->RemoveCondition(EConditionType::Invincible);
            StrongThis->IsStarEffecting = false;
            if (StrongThis->StarEffectsParticleComponent != nullptr)
            {
                StrongThis->StarEffectsParticleComponent->DestroyComponent();
            }
        }
    }, LifeTime, false);
}

void APlayer_Mario::OnDie()
{
    Super::OnDie();

    AController* controllerRef = GetController();

    Destroy();
    if (UWorld* world = GetWorld())
    {
        if (AHarioGameMode* gameMode = Cast<AHarioGameMode>(GetWorld()->GetAuthGameMode()))
        {
            gameMode->RestartPlayer(controllerRef);
        }
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
    	    UE_LOG(LogTemp, Warning, TEXT("무적상태 on"));
    	    
    	    TakeDamageMaterialHandler();  
    	    if (TakeDamageMaterialInstance)
    	    {
    	     
    	        
    	        TArray<UPrimitiveComponent*> Components;
    	        Components.Add(GetMesh());
    	        if (WornHatMesh)
    	        {
    	        UE_LOG(LogTemp, Log, TEXT("WornHatMesh 발견: %s"), *WornHatMesh->GetName());
    	        UE_LOG(LogTemp, Log, TEXT("WornHatMesh의 머티리얼 슬롯 수: %d"), WornHatMesh->GetNumMaterials());
    	            if (WornHatMesh->GetNumMaterials() == 0)
    	            {
    	                UE_LOG(LogTemp, Error, TEXT("WornHatMesh에 머티리얼이 없습니다! 기본 머티리얼을 할당하세요."));
    	            }
    	            Components.Add(WornHatMesh); // UStaticMeshComponent도 UPrimitiveComponent로 암묵적 캐스팅 가능
    	        }
    	        float BlinkDuration = 3.0f; // 깜빡임 지속 시간
    	        TakeDamageMaterialInstance->StartBlinkEffect(Components, BlinkTimerHandle, InvincibleLocalTimerHandle, BlinkDuration);
    	    }
    	}
        GetWorldTimerManager().SetTimer(InvincibleTimerHandle, this, &APlayer_Mario::RemoveInvincibility, 3.0f, false);
    	return ActualDamage;
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

void APlayer_Mario::OnBodyBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (false == IsStarEffecting)
    {
        return;
    }

    auto monster = Cast<ABaseMonster>(OtherActor);
    if (monster == nullptr)
    {
        return;
    }
    monster->RemoveCondition(EConditionType::Invincible);
    CalcDamage(10.f, this, monster);
}







