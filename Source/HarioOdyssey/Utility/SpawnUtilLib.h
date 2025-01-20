// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SpawnUtilLib.generated.h"

/**
 * 
 */
UCLASS()
class HARIOODYSSEY_API USpawnUtilLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// 지면 위치 찾기
	UFUNCTION(BlueprintCallable, Category = "Gameplay Utilities|World", meta = (WorldContext = "WorldContextObject"))
	static FVector GetGroundLocation(const UObject* WorldContextObject, const FVector& Location, float TraceDistance = 1000.f);
    
	// 지면 위치와 노멀 찾기
	UFUNCTION(BlueprintCallable, Category = "Gameplay Utilities|World", meta = (WorldContext = "WorldContextObject"))
	static bool GetGroundLocationAndNormal(const UObject* WorldContextObject, const FVector& Location, 
		FVector& OutLocation, FVector& OutNormal, float TraceDistance = 1000.f);
    
	UFUNCTION(BlueprintCallable, Category = "Gameplay Utilities|Spawning", meta = (WorldContext = "WorldContextObject"))
	static AActor* SpawnActorOnGround(const UObject* WorldContextObject, 
		TSubclassOf<AActor> ActorClass, 
		const FVector& Location,
		const FRotator& Rotation = FRotator::ZeroRotator);

	// 오브젝트가 지면에 있는지 확인
	UFUNCTION(BlueprintPure, Category = "Gameplay Utilities|World")
	static bool IsOnGround(AActor* Actor, float TraceDistance = 100.f);
};
