#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HarioOdyssey/AreaObject/Base/AreaObject.h"
#include "Player_Mario.generated.h"

class AHatProjectile;
class UCoinCounterWidget;
class UHealthWidget;


UCLASS()
class HARIOODYSSEY_API APlayer_Mario : public AAreaObject
{
	GENERATED_BODY()

public:
	// 기본 생성자
	APlayer_Mario();

protected:
	// 게임이 시작하거나 스폰될 때 호출됩니다.
	virtual void BeginPlay() override;
	
public:	
	// 매 프레임마다 호출됩니다.
	virtual void Tick(float DeltaTime) override;

	// 동전 획득 함수
	void AddCoin(int32 CoinValue);

	// 플레이어 입력 설정 함수
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 이동 함수
	void OnMoveForward(float Value);
	void OnMoveRight(float Value);

	// 카메라 회전 함수
	void OnTurn(float Value);
	void OnLookUp(float Value);
	void OnResetView() ; //카메라 시점 초기화 

	// 점프 함수
	void OnStartJump();
	
	// 모자 던지기와 받기 함수
	void OnThrowHat();
	
	//무적상태
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						   bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
						   const FHitResult& Hit) override;
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	


	
	// 깜빡이는 효과 처리 및 정리
	// void HandleBlinkingEffect(bool bStart);
	// void StartBlinkingEffect();
	
	
	//virtual void OnDie() override; //죽을 때 함수

private:
	// 카메라 붐 (Spring Arm)과 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* FollowCamera;

	//카메라 시점 초기화
	UPROPERTY(VisibleAnywhere)
	FRotator DefaultCameraRotation;

	// 동전 카운트
	int32 CoinCount = 0;

	//UI 생성
	TObjectPtr<UCoinCounterWidget> CoinCounterWidget= nullptr;
	TObjectPtr<UHealthWidget> HealthWidget= nullptr;
	

	//무적상태
	FTimerHandle InvincibleTimerHandle;
	void RemoveInvincibility();

	// 깜빡이는 상태 변수
	//bool bIsInvincible = false;

	// 깜빡이는 타이머 변수
	// FTimerHandle BlinkHandle;
	// float BlinkDuration; // 총 지속 시간
	// float BlinkInterval; // 깜빡이는 간격
	// float BlinkTimer;    // 현재까지 경과 시간
	//

	
protected:
	// 모자 던지기 관련 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hat")
	TSubclassOf<class AHatProjectile> HatClass;

	
};