#include "TakeDamageMaterial.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"


void UTakeDamageMaterial::InitializeAndApplyDynamicMaterial(UPrimitiveComponent* Component)
{
    if (!DynamicMaterialInstances.Contains(Component))  
    {
        int32 NumSlots = Component->GetNumMaterials();
        //TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMaterial = Component->CreateAndSetMaterialInstanceDynamic(0);



        // 머티리얼이 적용되었는지 로그 확인 => 모자 잘 불러와짐
        // if (Component->GetMaterial(0))
        // {
        //     UE_LOG(LogTemp, Log, TEXT("%s의 현재 머티리얼: %s"), *Component->GetName(), *Component->GetMaterial(0)->GetName());
        // }
        // else
        // {
        //     UE_LOG(LogTemp, Error, TEXT("%s에 머티리얼이 존재하지 않습니다!"), *Component->GetName());
        // }

        
        // 기본 머티리얼 로그 확인
        // UMaterialInterface* BaseMaterial = Component->GetMaterial(0);
        // if (!BaseMaterial)
        // {
        //     UE_LOG(LogTemp, Error, TEXT("%s에 기본 머티리얼이 없습니다! 기본 머티리얼을 설정합니다."), *Component->GetName());
        //     
        //     // 기본 머티리얼을 강제 적용 (머티리얼이 없는 경우)
        //     static ConstructorHelpers::FObjectFinder<UMaterialInterface> DefaultMat(TEXT("Material'/Game/Materials/M_DefaultMaterial.M_DefaultMaterial'"));
        //     if (DefaultMat.Succeeded())
        //     {
        //         Component->SetMaterial(0, DefaultMat.Object);
        //         BaseMaterial = DefaultMat.Object;
        //     }
        //     else
        //     {
        //         UE_LOG(LogTemp, Error, TEXT("기본 머티리얼 로드 실패!"));
        //         return; // 머티리얼이 없으면 동적 머티리얼을 생성할 수 없으므로 리턴
        //     }
        // }

        
        //  기존 머티리얼 저장 
        if (!OriginalMaterials.Contains(Component))
        {
            TArray<UMaterialInterface*> Materials;
            for (int32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
            {
                Materials.Add(Component->GetMaterial(SlotIndex));
            }
            OriginalMaterials.Add(Component,Materials);
            UE_LOG(LogTemp, Log, TEXT("원래 머티리얼 저장: %s"), *Component->GetName());
        }


        //동적 머티리얼 생성
        TArray<TWeakObjectPtr<UMaterialInstanceDynamic>> DynamicMaterials;
        for (int32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
        {
            UMaterialInstanceDynamic* DynamicMaterial = Component->CreateAndSetMaterialInstanceDynamic(SlotIndex);
            if (DynamicMaterial)
            {
                DynamicMaterials.Add(DynamicMaterial);
                Component->SetMaterial(SlotIndex, DynamicMaterial);
            }
        }
        DynamicMaterialInstances.Add(Component, DynamicMaterials);
        UE_LOG(LogTemp, Log, TEXT("Dynamic Material Instance 생성 및 적용 성공: %s"), *Component->GetName());
    }
            //=>CharacterMesh0, WaronHatMesh 잘 불러와짐 
    
}

void UTakeDamageMaterial::StartBlinkEffect(TArray<UPrimitiveComponent*> Components, FTimerHandle& BlinkTimerHandle, FTimerHandle& InvincibleLocalTimerHandle, float BlinkDuration)
{
    for (UPrimitiveComponent* Component : Components)
    {
        if (Component)
        {
            InitializeAndApplyDynamicMaterial(Component);
        }
    }

    if (Components.Num() > 0)
    {
        Components[0]->GetWorld()->GetTimerManager().SetTimer(BlinkTimerHandle, [this, Components]()
        {
            static bool bIsVisible = true;
            bIsVisible = !bIsVisible;
            float NewOpacity = bIsVisible ? 1.0f : 0.2f;
            //FLinearColor BlinkColor = bIsVisible ? FLinearColor::White : FLinearColor::Red; // 색상 변경

            for (UPrimitiveComponent* Component : Components)
            {
                if (Component && DynamicMaterialInstances.Contains(Component))
                {
                    TArray<TWeakObjectPtr<UMaterialInstanceDynamic>> DynamicMaterials = DynamicMaterialInstances[Component];
                    //TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMaterial = DynamicMaterialInstances[Component];
                    for (TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMaterial : DynamicMaterials)
                    {
                        if (DynamicMaterial.IsValid())
                        {
                            DynamicMaterial->BlendMode = EBlendMode::BLEND_Translucent;
                            DynamicMaterial->SetScalarParameterValue(FName("Opacity"), NewOpacity);
                        }
                    }
                    // if (DynamicMaterial.IsValid()) //=>wornhat안불러와짐
                    // {
                    //     DynamicMaterial->SetScalarParameterValue(FName("Opacity"), NewOpacity);
                    //     DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), BlinkColor); // 색상 변경 추가
                    // }
                }
            }
        }, 0.1f, true);

       //일정시간 후 기존 머티리얼로 볼구
        Components[0]->GetWorld()->GetTimerManager().SetTimer(InvincibleLocalTimerHandle, [this, Components, &BlinkTimerHandle]()
        {
            for (UPrimitiveComponent* Component : Components)
            {
                if (Component && OriginalMaterials.Contains(Component))
                {
                    //TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMaterial = DynamicMaterialInstances[Component];
                    TArray<UMaterialInterface*> OriginalMaterialArray = OriginalMaterials[Component];
                    int32 NumSlots = Component->GetNumMaterials();
                    for (int32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
                    {
                        if (OriginalMaterialArray.IsValidIndex(SlotIndex))
                        {
                            Component->SetMaterial(SlotIndex, OriginalMaterialArray[SlotIndex]);
                        }
                    }
                    UE_LOG(LogTemp, Log, TEXT("원래 머티리얼 복구 완료: %s"), *Component->GetName());
                    // if (DynamicMaterial.IsValid())
                    // {
                    //     //일정 시간 후 원래 머티리얼로 복구
                    //     Component->SetMaterial(0,OriginalMaterials[Component]);
                    //     
                    //     
                    //     DynamicMaterial->SetScalarParameterValue(FName("Opacity"), 1.0f);
                    //     DynamicMaterial->SetVectorParameterValue(FName("BaseColor"), FLinearColor::White); // 원래 색상 복구
                    // }
                }
            }

            if (Components[0])
            {
                Components[0]->GetWorld()->GetTimerManager().ClearTimer(BlinkTimerHandle);
            }

            UE_LOG(LogTemp, Warning, TEXT("무적 해제 및 깜빡임 종료"));
        }, BlinkDuration, false);
    }
}



//[case1]
// void UTakeDamageMaterial::InitializeAndApplyDynamicMaterial(UPrimitiveComponent* Component)
// {
//     if (!DynamicMaterialInstances.Contains(Component))  
//     {
//         TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMaterial = Component->CreateAndSetMaterialInstanceDynamic(0);
//         if (DynamicMaterial.IsValid())
//         {
//             DynamicMaterialInstances.Add(Component, DynamicMaterial);
//             int32 NumSlots = Component->GetNumMaterials();
//             for (int32 SlotIndex = 0; SlotIndex < NumSlots; ++SlotIndex)
//             {
//                 Component->SetMaterial(SlotIndex, DynamicMaterial.Get());
//             }
//             UE_LOG(LogTemp, Log, TEXT("Dynamic Material Instance 생성 및 전체 슬롯 적용 성공"));
//         }
//         else
//         {
//             UE_LOG(LogTemp, Error, TEXT("Dynamic Material Instance 생성 실패"));
//         }
//     }
// }
//
//
//
// void UTakeDamageMaterial::StartBlinkEffect(TArray<UPrimitiveComponent*> Components, FTimerHandle& BlinkTimerHandle, FTimerHandle& InvincibleLocalTimerHandle, float BlinkDuration)
// {
//     //[CASE2]
//     for (UPrimitiveComponent* Component : Components)
//     {
//         if (Component)
//         {
//             InitializeAndApplyDynamicMaterial(Component);
//         }
//     }
//     // 깜빡임 시작
//     if (Components.Num() > 0)
//     {
//         Components[0]->GetWorld()->GetTimerManager().SetTimer(BlinkTimerHandle, [this, Components]()
//         {
//             static bool bIsVisible = true;
//             bIsVisible = !bIsVisible;
//             float NewOpacity = bIsVisible ? 1.0f : 0.2f;
//
//             for (UPrimitiveComponent* Component : Components)
//             {
//                 if (Component && DynamicMaterialInstances.Contains(Component))
//                 {
//                     if (auto DynamicMaterial = DynamicMaterialInstances[Component])
//                     {
//                         DynamicMaterial->SetScalarParameterValue(FName("Opacity"), NewOpacity);
//                     }
//                 }
//             }
//         }, 0.1f, true);
//
//         // 일정 시간이 지난 후 복원
//         Components[0]->GetWorld()->GetTimerManager().SetTimer(InvincibleLocalTimerHandle, [this, Components, &BlinkTimerHandle]()
//         {
//             for (UPrimitiveComponent* Component : Components)
//             {
//                 if (Component && DynamicMaterialInstances.Contains(Component))
//                 {
//                     if (auto DynamicMaterial = DynamicMaterialInstances[Component])
//                     {
//                         DynamicMaterial->SetScalarParameterValue(FName("Opacity"), 1.0f);
//                     }
//                 }
//             }
//
//             // 타이머 클리어
//             if (Components[0])
//             {
//                 Components[0]->GetWorld()->GetTimerManager().ClearTimer(BlinkTimerHandle);
//                 Components[0]->GetWorld()->GetTimerManager().ClearTimer(InvincibleLocalTimerHandle);
//             }
//
//             UE_LOG(LogTemp, Warning, TEXT("무적 해제 및 깜빡임 종료"));
//         }, BlinkDuration, false);
//     }
//     
// }

    //[CASE1]
    // TWeakObjectPtr의 유효성 확인
    // if (!DynamicMaterialInstance.IsValid())  
    // {
    //     InitializeAndApplyDynamicMaterial(Mesh);
    // }
    //
    // if (DynamicMaterialInstance.IsValid())  // 유효성 확인
    // {
    //     // 깜빡임 효과 시작 (0.1초마다 투명도 토글)
    //     Mesh->GetWorld()->GetTimerManager().SetTimer(BlinkTimerHandle, [this]()
    //     {
    //         static bool bIsVisible = true;
    //         bIsVisible = !bIsVisible;
    //         float NewOpacity = bIsVisible ? 1.0f : 0.2f;
    //         DynamicMaterialInstance->SetScalarParameterValue(FName("Opacity"), NewOpacity);
    //     }, 0.1f, true);
    //
    //}, 2.0f, false);
          //    // }
    //     
    //    
    //     // 2초 후 무적 해제 및 깜빡임 멈춤
    //     Mesh->GetWorld()->GetTimerManager().SetTimer(InvincibleLocalTimerHandle, [this, Mesh,&BlinkTimerHandle]()
    //     {
    //         ensure(Mesh != nullptr && Mesh->GetWorld() != nullptr); //디버그 확인용
    //         Mesh->GetWorld()->GetTimerManager().ClearTimer(BlinkTimerHandle);
    //         if (DynamicMaterialInstance.IsValid())  // 유효성 확인
    //         {
    //             // Opacity를 1.0으로 복원
    //             DynamicMaterialInstance->SetScalarParameterValue(FName("Opacity"), 1.0f);
    //         }
    //         UE_LOG(LogTemp, Warning, TEXT("무적 해제 및 깜빡임 효과 종료"));
    //     
