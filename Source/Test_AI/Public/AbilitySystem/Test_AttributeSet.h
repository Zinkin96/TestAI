// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "Test_AttributeSet.generated.h"

/**
 *
 */
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class TEST_AI_API UTest_AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayAttributeData MaxHealth = 100.0f;
	ATTRIBUTE_ACCESSORS(UTest_AttributeSet, MaxHealth)

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayAttributeData CurrentHealth = 100.0f;
	ATTRIBUTE_ACCESSORS(UTest_AttributeSet, CurrentHealth)

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FGameplayAttributeData MeleeDamage = 0.0f;
	ATTRIBUTE_ACCESSORS(UTest_AttributeSet, MeleeDamage)

};