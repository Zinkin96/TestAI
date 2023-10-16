// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/GameplayEffects/GE_ApplyHealthChange.h"
#include "AbilitySystem/Test_AttributeSet.h"

UGE_ApplyHealthChange::UGE_ApplyHealthChange()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	FGameplayModifierInfo HealthChange;
	HealthChange.Attribute = UTest_AttributeSet::GetCurrentHealthAttribute();
	HealthChange.ModifierOp = EGameplayModOp::Additive;
	FSetByCallerFloat SetByCallerHandle;
	SetByCallerHandle.DataTag = FGameplayTag::RequestGameplayTag(FName("Values.HealthChange"));
	HealthChange.ModifierMagnitude = SetByCallerHandle;
	
	Modifiers.Add(HealthChange);
}