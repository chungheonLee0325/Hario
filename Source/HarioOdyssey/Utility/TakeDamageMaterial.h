#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "TakeDamageMaterial.generated.h"

class USkeletalMeshComponent;
struct FTimerHandle;

UCLASS()
class HARIOODYSSEY_API UTakeDamageMaterial : public UObject
{
	GENERATED_BODY()

public:
	void InitializeAndApplyDynamicMaterial(USkeletalMeshComponent* Mesh);
	void StartBlinkEffect(USkeletalMeshComponent* Mesh, FTimerHandle& BlinkTimerHandle, FTimerHandle& InvincibleLocalTimerHandle, float BlinkDuration);

private:
	// TWeakObjectPtr로 수정하여 가비지 컬렉션에 안전하도록 함
	TWeakObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance = nullptr;
};