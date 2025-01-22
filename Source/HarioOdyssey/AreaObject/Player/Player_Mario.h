#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HarioOdyssey/AreaObject/Base/AreaObject.h"
#include "Player_Mario.generated.h"

class AHatProjectile;
class UCoinCounterWidget;
class UHealthWidget;
class UTakeDamageMaterial;
class AHatProjectile;

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
	void OnStopJump();
	
	// 모자 던지기와 받기 함수
	void OnThrowHat();
	AHatProjectile* ActiveHat;

	
	//무적상태
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						   bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
						   const FHitResult& Hit) override;
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	void IsActorHidden();
	// TakeDamageMaterial 인스턴스를 생성하는 함수
	void TakeDamageMaterialHandler();
	
	//무적상태
	FTimerHandle BlinkTimerHandle;
	FTimerHandle InvincibleLocalTimerHandle;
	FTimerHandle InvincibleTimerHandle;
	void RemoveInvincibility();
	// TakeDamageMaterial 인스턴스
	UTakeDamageMaterial* TakeDamageMaterialInstance;

	
	//virtual void OnDie() override; //죽을 때 함수

private:
	// 카메라 붐 (Spring Arm)과 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;
	
	//bool bPressedJump;
	bool bIsJumping;
	//class UCameraComponent* bIsJumping=nullptr;

	// 카메라의 초기 위치와 회전 저장
	FTransform CameraInitialTransform;
	
	//카메라 시점 초기화
	UPROPERTY(VisibleAnywhere)
	FRotator DefaultCameraRotation;

	// 동전 카운트
	int32 CoinCount = 0;

	//UI 생성
	TObjectPtr<UCoinCounterWidget> CoinCounterWidget= nullptr;
	TObjectPtr<UHealthWidget> HealthWidget= nullptr;
	
	//무적상태 - 깜빡임
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterialInstance;

protected:
	// 모자 던지기 관련 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hat")
	TSubclassOf<class AHatProjectile> HatClass;

	UPROPERTY(EditAnywhere, Category = "Hat")
	USceneComponent* HatAttachPoint;
	
};
