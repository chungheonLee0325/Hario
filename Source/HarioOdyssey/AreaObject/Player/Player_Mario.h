#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HarioOdyssey/AreaObject/Base/AreaObject.h"
#include "Player_Mario.generated.h"

class UCoinCounterWidget;
class UHealthWidget;
class UTakeDamageMaterial;
class  AHatProjectile;
/**
 * 마리오 캐릭터 클래스
 * - 모자 던지기/회수
 * - 동전 획득
 * - 카메라 조작
 * - 무적 상태 등
 */

UCLASS()
class HARIOODYSSEY_API APlayer_Mario : public AAreaObject
{
	GENERATED_BODY()

public:
	// 기본 생성자
	APlayer_Mario();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
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



	
	//무적상태
	virtual float TakeDamage(float Damage, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void NotifyHit(UPrimitiveComponent* MyComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						   bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse,
						   const FHitResult& Hit) override;
	void RemoveInvincibility();
	

	// TakeDamageMaterial 인스턴스를 생성하는 함수
	void TakeDamageMaterialHandler();
	
public:
	// 동전 획득 함수
	void AddCoin(int32 CoinValue);
	
	static int32 m_AreaObjectID;

	
private:
	//  카메라 컴포넌트
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComponent;

	/*모자 [Case3]*/
	// 모자 클래스 레퍼런스
	UPROPERTY(EditAnywhere, Category = "Hat")
	TSubclassOf<AHatProjectile> HatClass;

	// 모자 인스턴스
	UPROPERTY()
	AHatProjectile* HatInstance;

	// 모자 투척 여부
	bool bIsHatThrown;

	// 모자 던지기 위치와 방향
	UPROPERTY(EditAnywhere, Category = "Hat")
	FVector HatSpawnOffset;

	// 모자가 이동하는 거리
	UPROPERTY(EditAnywhere, Category = "Hat")
	float MaxHatThrowDistance;

	// 모자 던지는 속도
	UPROPERTY(EditAnywhere, Category = "Hat")
	float HatThrowSpeed;

	// 모자가 돌아오는 속도
	UPROPERTY(EditAnywhere, Category = "Hat")
	float HatRecallSpeed;



	
	// 모자 메시 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Hat", meta=(AllowPrivateAccess="true"))
	USkeletalMeshComponent* HatMesh;
	
	// 모자를 다시 부착할 지점
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Hat", meta=(AllowPrivateAccess="true"))
	USceneComponent* HatAttachPoint;

	// 모자 던졌는지 여부
	//bool bIsHatThrown = false;

	// 모자 던졌을 때, 돌아오기 및 재부착을 위한 타이머 핸들
	FTimerHandle HatReturnTimerHandle;
	FTimerHandle HatReattachTimerHandle;
	
	//점프 상태
	bool bIsJumping = false;

	//UI 생성
	TObjectPtr<UCoinCounterWidget> CoinCounterWidget= nullptr;
	TObjectPtr<UHealthWidget> HealthWidget= nullptr;

	// 동전 카운트
	int32 CoinCount = 0;
	
	// 카메라의 초기 위치, 회전 저장
	FTransform CameraInitialTransform;
	
	//무적상태
	FTimerHandle BlinkTimerHandle;
	FTimerHandle InvincibleLocalTimerHandle;
	FTimerHandle InvincibleTimerHandle;
	
	//카메라 시점 초기화
	UPROPERTY(VisibleAnywhere)
	FRotator DefaultCameraRotation;
	
	// TakeDamageMaterial 인스턴스
	UTakeDamageMaterial* TakeDamageMaterialInstance=nullptr;
	
	//무적상태 - 깜빡임
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterialInstance;

	//무적상태 해제
	void IsActorHidden();

protected:
	void RemoveInvincibilityTimer();
};
