// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystem/GameplayEffects/GE_ApplyHealthChange.h"
#include "Test_Ability_UseHealPotion.generated.h"

/**
 * 
 */
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayEvent;

UCLASS()
class TEST_AI_API UTest_Ability_UseHealPotion : public UGameplayAbility
{
	GENERATED_BODY()
	
public:

	UTest_Ability_UseHealPotion();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TObjectPtr<UAnimMontage> AnimMontageAsset = LoadObject<UAnimMontage>(nullptr, TEXT("/Script/Engine.AnimMontage'/Game/BossyEnemy/Animations/Boss_PotionUse_Montage.Boss_PotionUse_Montage'"));

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TSubclassOf<UGameplayEffect> GameplayEffectClass = UGE_ApplyHealthChange::StaticClass();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FGameplayEffectSpecHandle SpecHandle;

	UFUNCTION()
		void RestoreHealth(FGameplayEventData Payload);

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;

	virtual bool CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) override;

	UAbilitySystemComponent* AbilitySystemComponent;

	FGameplayEffectSpecHandle UsePotionHandle;

	UAbilityTask_PlayMontageAndWait* PlayMontageAndWait;

	UAbilityTask_WaitGameplayEvent* WaitGameplayEvent;
};
