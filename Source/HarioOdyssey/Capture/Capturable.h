// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Capturable.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UCapturable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class HARIOODYSSEY_API ICapturable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, Category = "Capture")
	void OnCaptureStart();

	UFUNCTION(BlueprintNativeEvent, Category = "Capture")
	void WhileCaptured(class ACharacter* CaptureOwner);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Capture")
	void OnCaptureEnd();
	
	UFUNCTION(BlueprintNativeEvent, Category = "Capture")
	bool CanBeCaptured();


	protected:
	// Todo 고민... 별도 component로 뺄지 -> 모작 프로젝트에서는 애매한듯 변수도 없애는 방향성으로 진행 
	int CaptureEnableCount = 0;
	bool CanCapture = true;

public:
	
};
