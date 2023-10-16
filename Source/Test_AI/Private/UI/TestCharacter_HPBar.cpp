// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TestCharacter_HPBar.h"
#include "Kismet/KismetMathLibrary.h"

void UTestCharacter_HPBar::SetHP(float MaxHealth, float CurrentHealth)
{
	float Value = UKismetMathLibrary::MapRangeClamped(CurrentHealth, 0.0f , MaxHealth, 0.0f, 1.0f);
	HPBar->SetPercent(Value);
}