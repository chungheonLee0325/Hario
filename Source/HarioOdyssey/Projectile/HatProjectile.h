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
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

	
	
	// 던져지는 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	FVector ThrowVelocity;

	// 모자가 달라붙을 몬스터의 범위
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	float AttachDistance = 300.f;

	// 모자가 붙을 때 몬스터 위쪽 위치를 조정할 오프셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Projectile")
	FVector AttachOffset = FVector(0.f, 0.f, 50.f);

	// 모자가 붙을 수 있는 몬스터 객체 (타겟)
	UPROPERTY()
	AActor* TargetMonster;

	// 모자 속성 (물리적 효과 등)
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* HatMesh;

	// 타겟을 찾고 가까운지 확인하는 함수
	void CheckAndAttachToMonster();
    
	// 몬스터와의 거리 계산
	bool IsNearMonster();

	UPROPERTY(VisibleAnywhere)
	class USphereComponent* CollisionComponent;

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* ProjectileMovement;

	UFUNCTION()
	void OnHit(class UPrimitiveComponent* HitComp, AActor* OtherActor, 
			   class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
			   bool bFromSweep, const FHitResult& SweepResult);

	// 몬스터와 붙을 때 호출될 함수
	void AttachToMonster(ABaseMonster* Monster);

	// 플레이어를 일시적으로 숨기기
	void HidePlayerTemporarily();

	// 일시적으로 숨겨진 플레이어가 다시 보이도록 하기
	void ShowPlayerAgain();

	// 숨김을 해제할 시간 (초 단위)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player Visibility")
	float HiddenDuration = 5.0f;  // 5초 동안 숨김

	// 플레이어 참조
	APlayer_Mario* Player;

private:
	// 숨김 타이머
	FTimerHandle UnhideTimerHandle;

	

};