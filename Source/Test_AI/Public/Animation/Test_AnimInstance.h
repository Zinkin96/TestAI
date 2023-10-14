// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayEffectTypes.h"
#include "Test_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TEST_AI_API UTest_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
		FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;

};
