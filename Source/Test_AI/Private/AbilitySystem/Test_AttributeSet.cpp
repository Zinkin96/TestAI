// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Test_AttributeSet.h"
#include "Kismet/KismetMathLibrary.h"

void UTest_AttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	if (Attribute == GetHealthAttribute())
	{
		NewValue = UKismetMathLibrary::Max(0.0f, NewValue);
	}
}
