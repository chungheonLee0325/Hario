#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HatProjectile.generated.h"

class ABaseMonster;  // AMonster 클래스의 포워드 선언
class APlayer_Mario;  // 플레이어 클래스 포워드 선언

// 클래스 선언
UCLASS()
class HARIOODYSSEY_API AHatProjectile : public AActor
{
	GENERATED_BODY()
    
public:    
	AHatProjectile();

protected:
	virtual void BeginPlay() override;

public:    
	virtual void Tick(float DeltaTime) override;

	// 모자 초기화 함수
	void InitializeHat(FVector ForwardDirection);
	

private:
	FVector InitialDirection; // 모자가 날아갈 방향
	FVector StartLocation;    // 모자의 시작 위치
	float Speed;              // 모자의 속도
	float FlightTime;         // 날아가는 시간
	float CurrentTime;        // 현재 경과 시간

	bool bReturning;          // 모자가 돌아오는 상태인지 여부
	AActor* OwnerActor;       // 모자를 던진 주인 (Player_Mario)
	

};