#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TakeDamageMaterial.generated.h"

class USkeletalMeshComponent;
struct FTimerHandle;
class TakeDamageMaterial;

UCLASS()
class HARIOODYSSEY_API UTakeDamageMaterial : public UObject
{
	GENERATED_BODY()

public:
	void InitializeAndApplyDynamicMaterial(UPrimitiveComponent* Component);
	void StartBlinkEffect(TArray<UPrimitiveComponent*> Components, FTimerHandle& BlinkTimerHandle, FTimerHandle& InvincibleLocalTimerHandle, float BlinkDuration);

private:
	TMap<UPrimitiveComponent*, TArray<TWeakObjectPtr<UMaterialInstanceDynamic>>> DynamicMaterialInstances;
	TMap<UPrimitiveComponent*, TArray<UMaterialInterface*>> OriginalMaterials; // 원래 머티리얼 저장
};