// Fill out your copyright notice in the Description page of Project Settings.
#include "SpawnUtilLib.h"

FVector USpawnUtilLib::GetGroundLocation(const UObject* WorldContextObject, const FVector& Location,
                                         float TraceDistance)
{
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject,
	                                                             EGetWorldErrorMode::LogAndReturnNull))
	{
		FHitResult HitResult;
		FVector Start = Location + FVector(0, 0, TraceDistance);
		FVector End = Location - FVector(0, 0, TraceDistance);

		FCollisionQueryParams QueryParams;
		QueryParams.bTraceComplex = false;
		QueryParams.AddIgnoredActor(Cast<AActor>(WorldContextObject));
		
		if (World->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldStatic, QueryParams))
		{
			return HitResult.Location;
		}
	}
	return Location;
}

bool USpawnUtilLib::GetGroundLocationAndNormal(const UObject* WorldContextObject, const FVector& Location,
                                               FVector& OutLocation, FVector& OutNormal, float TraceDistance)
{
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject,
	                                                             EGetWorldErrorMode::LogAndReturnNull))
	{
		FHitResult HitResult;
		FVector Start = Location + FVector(0, 0, TraceDistance);
		FVector End = Location - FVector(0, 0, TraceDistance);

		FCollisionQueryParams QueryParams;
		QueryParams.bTraceComplex = false;

		if (World->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldStatic, QueryParams))
		{
			OutLocation = HitResult.Location;
			OutNormal = HitResult.Normal;
			return true;
		}
	}
	return false;
}

AActor* USpawnUtilLib::SpawnActorOnGround(const UObject* WorldContextObject,
                                          TSubclassOf<AActor> ActorClass,
                                          const FVector& Location,
                                          const FRotator& Rotation)
{
	if (!ActorClass) return nullptr;

	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		FVector SpawnLocation = GetGroundLocation(WorldContextObject, Location);
		return World->SpawnActor<AActor>(ActorClass, SpawnLocation, Rotation);
	}

	return nullptr;
}

bool USpawnUtilLib::IsOnGround(AActor* Actor, float TraceDistance)
{	
	if (!Actor)
		return false;

	FHitResult HitResult;
	const FVector Start = Actor->GetActorLocation();
	const FVector End = Start - FVector(0, 0, TraceDistance);

	FCollisionQueryParams QueryParams;
	QueryParams.bTraceComplex = false;
	QueryParams.AddIgnoredActor(Actor);  // 자기 자신은 무시

	if (Actor->GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldStatic, QueryParams))
	{
		// 원하는 경우 거리를 체크
		// return HitResult.Distance < SomeThreshold;
		return true;
	}

	return false;
}
