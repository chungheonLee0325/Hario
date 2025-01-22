#include "TakeDamageMaterial.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

void UTakeDamageMaterial::InitializeAndApplyDynamicMaterial(USkeletalMeshComponent* Mesh)
{
    // 메시의 첫 번째 슬롯을 기준으로 동적 머티리얼 생성
    if (!DynamicMaterialInstance.IsValid())  // TWeakObjectPtr의 유효성 확인
    {
        DynamicMaterialInstance = Mesh->CreateAndSetMaterialInstanceDynamic(0);
        if (DynamicMaterialInstance.IsValid())  // 유효성 확인
        {
            // 메시의 모든 슬롯에 동일한 머티리얼 적용
            int32 NumSlots = Mesh->GetNumMaterials();
            for (int32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
            {
                Mesh->SetMaterial(SlotIndex, DynamicMaterialInstance.Get());
            }
            UE_LOG(LogTemp, Log, TEXT("Dynamic Material Instance 생성 및 전체 슬롯 적용 성공"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("Dynamic Material Instance 생성 실패"));
        }
    }
}

void UTakeDamageMaterial::StartBlinkEffect(USkeletalMeshComponent* Mesh, FTimerHandle& BlinkTimerHandle, FTimerHandle& InvincibleLocalTimerHandle)
{
    // TWeakObjectPtr의 유효성 확인
    if (!DynamicMaterialInstance.IsValid())  
    {
        InitializeAndApplyDynamicMaterial(Mesh);
    }

    if (DynamicMaterialInstance.IsValid())  // 유효성 확인
    {
        // 깜빡임 효과 시작 (0.1초마다 투명도 토글)
        Mesh->GetWorld()->GetTimerManager().SetTimer(BlinkTimerHandle, [this]()
        {
            static bool bIsVisible = true;
            bIsVisible = !bIsVisible;
            float NewOpacity = bIsVisible ? 1.0f : 0.2f;
            DynamicMaterialInstance->SetScalarParameterValue(FName("Opacity"), NewOpacity);
        }, 0.1f, true);


        
       
        // 2초 후 무적 해제 및 깜빡임 멈춤
        Mesh->GetWorld()->GetTimerManager().SetTimer(InvincibleLocalTimerHandle, [this, Mesh,&BlinkTimerHandle]()
        {
            ensure(Mesh != nullptr && Mesh->GetWorld() != nullptr); //디버그 확인용
            Mesh->GetWorld()->GetTimerManager().ClearTimer(BlinkTimerHandle);
            if (DynamicMaterialInstance.IsValid())  // 유효성 확인
            {
                // Opacity를 1.0으로 복원
                DynamicMaterialInstance->SetScalarParameterValue(FName("Opacity"), 1.0f);
            }
            UE_LOG(LogTemp, Warning, TEXT("무적 해제 및 깜빡임 효과 종료"));
        }, 2.0f, false);
    }
}