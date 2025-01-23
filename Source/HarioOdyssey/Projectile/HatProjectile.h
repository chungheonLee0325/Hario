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

	// 모자 초기화 함수
	void InitializeHat(FVector Direction, APlayer_Mario* NewOwner);

	APlayer_Mario* GetOwnerActor() const { return OwnerActor; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	FVector InitialDirection; // 모자 방향
	float Speed = 1000.0f; // 모자 속도

	//모자 비행 시간
	float FlightTime = 2.0f; // 날아가는 시간
	float CurrentTime = 0.0f; // 현재 경과 시간

	UPROPERTY()
	APlayer_Mario* OwnerActor; // 모자를 던진 주인 (Player_Mario)

	bool bReturning; // 모자가 돌아오는 상태인지 여부

	// 루트
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxComp;

	//모자 메시 컴포넌트
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* HatMesh;

	FVector StartLocation; // 모자의 시작 위치
};
