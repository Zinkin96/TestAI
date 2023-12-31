// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Test_Ability_UseHealPotion.h"
#include "AbilitySystemComponent.h"
#include "Test_GeneralPawnInterface.h"
#include "AbilitySystem/Test_AttributeSet.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"

UTest_Ability_UseHealPotion::UTest_Ability_UseHealPotion()
{
	AbilityTags.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability.HealPotion")));
	BlockAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));
	CancelAbilitiesWithTag.AddTag(FGameplayTag::RequestGameplayTag(FName("Ability")));
}

void UTest_Ability_UseHealPotion::RestoreHealth(FGameplayEventData Payload)
{
	AbilitySystemComponent = GetAbilitySystemComponentFromActorInfo();
	const UTest_AttributeSet* AttributeSet = AbilitySystemComponent->GetSet<UTest_AttributeSet>();
	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	UsePotionHandle = AbilitySystemComponent->MakeOutgoingSpec(GameplayEffectClass, 1.0f, EffectContext);
	UsePotionHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Values.HealthChange")), ITest_GeneralPawnInterface::Execute_GetHealthPotionsHealthRestore(GetOwningActorFromActorInfo()));;

	FGameplayAbilityTargetData_ActorArray* TargetData = new FGameplayAbilityTargetData_ActorArray();
	TargetData->TargetActorArray.Add(GetOwningActorFromActorInfo());
	FGameplayAbilityTargetDataHandle TargetDataHandle = FGameplayAbilityTargetDataHandle(TargetData);

	ApplyGameplayEffectSpecToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, UsePotionHandle, TargetDataHandle);
	PlayMontageAndWait->OnCompleted.RemoveDynamic(this, &UTest_Ability_UseHealPotion::K2_EndAbility);
	WaitGameplayEvent->EventReceived.RemoveDynamic(this, &UTest_Ability_UseHealPotion::RestoreHealth);
	CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
}

void UTest_Ability_UseHealPotion::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	PlayMontageAndWait = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, AnimMontageAsset);
	PlayMontageAndWait->OnCompleted.AddDynamic(this, &UTest_Ability_UseHealPotion::K2_EndAbility);
	PlayMontageAndWait->Activate();

	WaitGameplayEvent = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, FGameplayTag::RequestGameplayTag(FName("Effect.ApplyHealthChange")), NULL, true);
	WaitGameplayEvent->EventReceived.AddDynamic(this, &UTest_Ability_UseHealPotion::RestoreHealth);
	WaitGameplayEvent->Activate();
}

bool UTest_Ability_UseHealPotion::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, OUT FGameplayTagContainer* OptionalRelevantTags) const
{
	AActor* const AvatarActor = ActorInfo ? ActorInfo->AvatarActor.Get() : nullptr;
	if (!AvatarActor)
	{
		return false;
	}

	if (ITest_GeneralPawnInterface::Execute_GetHealthPotionsCount(AvatarActor) <= 0)
	{
		return false;
	}

	const UTest_AttributeSet* AttributeSet = ActorInfo->AbilitySystemComponent.Get()->GetSet<UTest_AttributeSet>();
	if (!AttributeSet)
	{
		return false;
	}
	if (AttributeSet->GetCurrentHealth() >= AttributeSet->GetMaxHealth())
	{
		return false;
	}

	Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	return true;
}

bool UTest_Ability_UseHealPotion::CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, OUT FGameplayTagContainer* OptionalRelevantTags)
{
	int32 PotionsCount = ITest_GeneralPawnInterface::Execute_GetHealthPotionsCount(GetOwningActorFromActorInfo());
	ITest_GeneralPawnInterface::Execute_SetHealthPotionsCount(GetOwningActorFromActorInfo(), --PotionsCount);
	Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags);
	return true;
}


