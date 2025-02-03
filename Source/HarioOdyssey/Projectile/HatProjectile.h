#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HatProjectile.generated.h"

class ABaseMonster; // AMonster 클래스의 포워드 선언
class APlayer_Mario; // 플레이어 클래스 포워드 선언

// 클래스 선언
UCLASS()
class HARIOODYSSEY_API AHatProjectile : public AActor
{
	GENERATED_BODY()

public:
	AHatProjectile();
	
	virtual void Tick(float DeltaTime) override;
	
	// 모자 초기화 함수
	void InitializeHat(FVector Direction, APlayer_Mario* NewOwner,bool bIsSpinThrow);
	void ResetScale();


	// 모자 상태 (던지는 중, 돌아오는 중 등)
	bool bReturning = false;
	bool bSpinning = false;

	// UFUNCTION()
	// void OnHatReturned(AActor* DestroyedActor);
	
	// 모자 속도 조절
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hat")
	float MaxHatThrowDistance = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Hat")
	float MaxHatThrowSpeed = 1500.f;
	bool bExpanding;
	float MaxOrbitRadius;
	float SpiralSpeed;
	float ExpansionTime;
	bool bIsThrownStraight;


	APlayer_Mario* GetOwnerActor() const { return OwnerActor; }

protected:
	virtual void BeginPlay() override;
	

	

private:

	// 메시 컴포넌트
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* HatMesh;

	FVector InitialDirection; // 모자 초기 방향
	float Speed = 750.0f; // 모자 속도

	//비행 시간
	float FlightTime = 0.8f; // 날아가는 시간
	float CurrentTime = 0.0f; // 현재 경과 시간

	UPROPERTY()
	APlayer_Mario* OwnerActor; // 모자를 던진 주인 (Player_Mario)
	
	
	//루트
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxComp;
	
	
	//회전
	float OrbitRadius= 200.0f;;       // 회전 궤도 반지름
	float OrbitSpeed= 180.f;;        // 회전 속도
	float CurrentAngle = 0.0f;      // 현재 회전 각도
	
	FVector StartLocation; // 모자의 시작 위치

	//목표 지점 모자 크기
	FTimerHandle ScaleTimerHandle;
	bool bHasScaled = false;
};
