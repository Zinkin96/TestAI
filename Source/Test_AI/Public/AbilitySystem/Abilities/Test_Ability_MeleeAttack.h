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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TObjectPtr<UAnimMontage> AnimMontageAsset = LoadObject<UAnimMontage>(nullptr, TEXT("/Script/Engine.AnimMontage'/Game/BossyEnemy/Animations/Boss_Attack_Uppercut_InP_Montage.Boss_Attack_Uppercut_InP_Montage'"));

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TSubclassOf<UGameplayEffect> GameplayEffectClass = UGE_ApplyHealthChange::StaticClass();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FGameplayEffectSpecHandle SpecHandle;

	UPROPERTY()
		float AttackRange = 120.0f;


	UAbilitySystemComponent* AbilitySystemComponent;

	FGameplayEffectSpecHandle MeleeAttackHandle;

	UFUNCTION()
	void DealDamage(FGameplayEventData Payload);

	bool CheckDistance(AActor* const AvatarActor) const;

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, OUT FGameplayTagContainer* OptionalRelevantTags = nullptr) const;
};
