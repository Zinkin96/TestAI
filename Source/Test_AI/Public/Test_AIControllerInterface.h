// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "Test_AIControllerInterface.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI)
class UTest_AIControllerInterface : public UInterface
{
	GENERATED_BODY()
	
};

/**
 *
 */
class TEST_AI_API ITest_AIControllerInterface
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		TMap<int32, FGameplayTag> GetAbilityPriority() const;

};
