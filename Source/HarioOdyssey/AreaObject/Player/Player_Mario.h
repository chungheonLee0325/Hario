#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HarioOdyssey/AreaObject/Base/AreaObject.h"
#include "Player_Mario.generated.h"

class AHatProjectile;
class UCoinCounterWidget;

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
	TObjectPtr<UCoinCounterWidget> CoinCounterWidget;
	

protected:
	// 모자 던지기 관련 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hat")
	TSubclassOf<class AHatProjectile> HatClass;
};