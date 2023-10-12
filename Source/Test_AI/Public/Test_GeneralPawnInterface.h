// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Test_GeneralPawnInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTest_GeneralPawnInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TEST_AI_API ITest_GeneralPawnInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void SetTarget(AActor* Target);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		AActor* GetTarget();
};
