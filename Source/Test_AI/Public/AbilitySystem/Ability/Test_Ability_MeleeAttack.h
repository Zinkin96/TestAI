// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/GameplayEffects/GE_ApplyHealthChange.h"
#include "Test_Ability_MeleeAttack.generated.h"

/**
 *
 */
UCLASS()
class TEST_AI_API UTest_Ability_MeleeAttack : public UGameplayAbility
{
	GENERATED_BODY()

public:

	UTest_Ability_MeleeAttack();

protected:

	UPROPERTY()
		TSubclassOf<UGameplayEffect> GameplayEffectClass = UGE_ApplyHealthChange::StaticClass();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FGameplayEffectSpecHandle SpecHandle;

	virtual void CommitExecute(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;

	UPROPERTY()
		UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY()
		FGameplayEffectSpecHandle MeleeAttackHandle;
};
